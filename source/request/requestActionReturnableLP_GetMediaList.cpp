
#include <raumserver/request/requestActionReturnableLP_GetMediaList.h>

namespace Raumserver
{
    namespace Request
    {

        RequestActionReturnableLongPolling_GetMediaList::RequestActionReturnableLongPolling_GetMediaList(std::string _url) : RequestActionReturnableLongPolling(_url)
        {
            action = RequestActionType::RAA_GETMEDIALIST;
            listRetrieved = false;
        }


        RequestActionReturnableLongPolling_GetMediaList::RequestActionReturnableLongPolling_GetMediaList(std::string _path, std::string _query) : RequestActionReturnableLongPolling(_path, _query)
        {
            action = RequestActionType::RAA_GETMEDIALIST;
            listRetrieved = false;
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
            auto id = getOptionValue("id");
            if (id == _listId)
                listRetrieved = true;            
        }
        

        bool RequestActionReturnableLongPolling_GetMediaList::executeActionLongPolling()
        {      
            auto id = getOptionValue("id");                     
            std::string lpid = getOptionValue("updateId");
            
            // if we do no long polling on a list id we havent loaded it yet, so for this we have to tell the manager
            // that he has to load the stuff
            if (lpid.empty())
            {
                connections.connect(managerEngineer->getMediaListManager()->sigMediaListDataChanged, this, &RequestActionReturnableLongPolling_GetMediaList::onMediaListDataChanged);
                managerEngineer->getMediaListManager()->loadMediaItemListByContainerId(id);

                while (!listRetrieved)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            }

            // after list is ready we get it and we create the json return
            auto mediaList = managerEngineer->getMediaListManager()->getList(id);

            rapidjson::StringBuffer jsonStringBuffer;
            rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonStringBuffer);
            
            addMediaListToJson(id, mediaList, jsonWriter);

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
                // todo: get from Media object itself?!?!
                _jsonWriter.StartObject();
                _jsonWriter.Key("id"); _jsonWriter.String(mediaItem->id.c_str());
                _jsonWriter.Key("parentId"); _jsonWriter.String(mediaItem->parentId.c_str());
                _jsonWriter.Key("id"); _jsonWriter.String(mediaItem->raumfeldName.c_str());
                _jsonWriter.EndObject();
            }
            _jsonWriter.EndArray();
            _jsonWriter.EndObject();
        }
    }
}

