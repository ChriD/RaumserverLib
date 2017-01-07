
#include <raumserver/request/requestActionReturnableLP_GetMediaList.h>
#include <raumserver/json/mediaItemJsonCreator.h>

namespace Raumserver
{
    namespace Request
    {

        RequestActionReturnableLongPolling_GetMediaList::RequestActionReturnableLongPolling_GetMediaList(std::string _url) : RequestActionReturnableLongPolling(_url)
        {
            action = RequestActionType::RAA_GETMEDIALIST;
            listRetrieved = false;
            formatedContainerId = "";
        }


        RequestActionReturnableLongPolling_GetMediaList::RequestActionReturnableLongPolling_GetMediaList(std::string _path, std::string _query) : RequestActionReturnableLongPolling(_path, _query)
        {
            action = RequestActionType::RAA_GETMEDIALIST;
            listRetrieved = false;
            formatedContainerId = "";
        }


        RequestActionReturnableLongPolling_GetMediaList::~RequestActionReturnableLongPolling_GetMediaList()
        {
        }


        bool RequestActionReturnableLongPolling_GetMediaList::isValid()
        {
            bool isValid = RequestActionReturnableLongPolling::isValid();

            auto id = getOptionValue("id");
            auto zoneId = getOptionValue("zoneid");

            if (id.empty())
            {
                logError("'id' option is needed to execute 'getMediaList' command!", CURRENT_FUNCTION);
                isValid = false;
            }            

            return isValid;
        }


        std::string RequestActionReturnableLongPolling_GetMediaList::getLastUpdateId()
        {              
            auto id = getOptionValue("id");
            auto lastUpdateId = getManagerEngineer()->getMediaListManager()->getLastUpdateIdForList(id);
            return lastUpdateId;
        }


        void RequestActionReturnableLongPolling_GetMediaList::onMediaListDataChanged(std::string _listId)
        {            
            if (!_listId.empty() && formatedContainerId == _listId)
                listRetrieved = true;            
        }
        

        bool RequestActionReturnableLongPolling_GetMediaList::executeActionLongPolling()
        {      
            auto id = getOptionValue("id");                     
            auto useCacheOption = getOptionValue("useCache");
            std::string lpid = getOptionValue("updateId");
            bool useCache = (useCacheOption == "1" || useCacheOption == "true") ? true : false;
            bool listGotFromCache = false;            

            std::vector<std::shared_ptr<Raumkernel::Media::Item::MediaItem>> mediaList;
            
            // if we do no long polling on a list id we haven't loaded it yet, so for this we have to tell the manager
            // that he has to load the stuff
            if (lpid.empty())
            {
                connections.connect(managerEngineer->getMediaListManager()->sigMediaListDataChanged, this, &RequestActionReturnableLongPolling_GetMediaList::onMediaListDataChanged); 

                // the id has to be formated well. That measn that the part after the last "/" has to be encoded
                auto parts = Raumkernel::Tools::StringUtil::explodeString(id, "/");
                if (parts.size() > 1)
                {
                    parts[parts.size() - 1] = Raumkernel::Tools::UriUtil::encodeUriPart(parts[parts.size() - 1]);
                    for (auto part : parts)
                    {
                        if (!formatedContainerId.empty())
                            formatedContainerId += "/";
                        formatedContainerId += part;
                    }
                }
                else
                {
                    formatedContainerId = id;
                }             
       
                // we do have a simple cache option whcih will look if there is already a list with items loaded into the
                // media list manager. The drawback of the simple caching is, that if the size of the list is 0 there is no caching
                if (useCache)
                {
                    mediaList = managerEngineer->getMediaListManager()->getList(formatedContainerId);
                    if (!mediaList.size())                    
                        managerEngineer->getMediaListManager()->loadMediaItemListByContainerId(formatedContainerId);
                    else
                        listGotFromCache = true;
                }
                // Caching is disabled, we inform the media list manager that he has to load/reload the list
                else
                {
                    managerEngineer->getMediaListManager()->loadMediaItemListByContainerId(formatedContainerId);
                }

                // wait till the list is loaded by the media list manager. if the list was loaded from cache
                // we do not have to wait for it (would lead to an endless loop)
                while (!listRetrieved && !listGotFromCache)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            }

            // now the list is ready, no matter if t was retrieved by the media list manager or if it was loaded
            // from the cache. If it was loaded from the cache (listGotFromCache) we do not need to get it again from the media manager
            if (!listGotFromCache)
                mediaList = managerEngineer->getMediaListManager()->getList(formatedContainerId);

            rapidjson::StringBuffer jsonStringBuffer;
            rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonStringBuffer);
            
            addMediaListToJson(formatedContainerId, mediaList, jsonWriter);

            setResponseData(jsonStringBuffer.GetString());

            return true;
         
        }

        void RequestActionReturnableLongPolling_GetMediaList::addMediaListToJson(const std::string &_id, std::vector<std::shared_ptr<Raumkernel::Media::Item::MediaItem>> &_mediaList, rapidjson::Writer<rapidjson::StringBuffer> &_jsonWriter)
        {
            _jsonWriter.StartObject();
            _jsonWriter.Key("id"); _jsonWriter.String(_id.c_str());
            _jsonWriter.Key("items");
            _jsonWriter.StartArray();
            for (auto mediaItem : _mediaList)
            {                
                _jsonWriter.StartObject();
                MediaItemJsonCreator::addJson(mediaItem, _jsonWriter);
                _jsonWriter.EndObject();
            }
            _jsonWriter.EndArray();
            _jsonWriter.EndObject();
        }
    }
}

