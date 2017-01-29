
#include <raumserver/request/requestActionReturnableLP_GetZoneMediaList.h>
#include <raumserver/json/mediaItemJsonCreator.h>

namespace Raumserver
{
    namespace Request
    {

        RequestActionReturnableLongPolling_GetZoneMediaList::RequestActionReturnableLongPolling_GetZoneMediaList(std::string _url) : RequestActionReturnableLongPolling(_url)
        {
            action = RequestActionType::RAA_GETZONEMEDIALIST;
            listRetrieved = false;
        }


        RequestActionReturnableLongPolling_GetZoneMediaList::RequestActionReturnableLongPolling_GetZoneMediaList(std::string _path, std::string _query) : RequestActionReturnableLongPolling(_path, _query)
        {
            action = RequestActionType::RAA_GETZONEMEDIALIST;
            listRetrieved = false;
        }


        RequestActionReturnableLongPolling_GetZoneMediaList::~RequestActionReturnableLongPolling_GetZoneMediaList()
        {
        }


        bool RequestActionReturnableLongPolling_GetZoneMediaList::isValid()
        {
            bool isValid = RequestActionReturnableLongPolling::isValid();            
            return isValid;
        }


        std::string RequestActionReturnableLongPolling_GetZoneMediaList::getLastUpdateId()
        {
            std::string lastUpdateIdCur, rendererUDN, lastUpdateIdSum;
            std::uint64_t lastUpdateSum = 0;
            auto id = getOptionValue("id");

            if (!id.empty())
            {
                auto mediaRenderer = getVirtualMediaRenderer(id);
                if (!mediaRenderer)
                    return "";

                std::string zonePlaylistId = Raumkernel::Manager::LISTID_ZONEIDENTIFIER + mediaRenderer->getUDN();
                lastUpdateIdSum = getManagerEngineer()->getMediaListManager()->getLastUpdateIdForList(zonePlaylistId);
            }
            // run through the renderers and get current update id by summing up the values
            // if the value is other than given in header something has changed.             
            else
            {
                auto zoneInfoMap = getManagerEngineer()->getZoneManager()->getZoneInformationMap();
                for (auto it : zoneInfoMap)
                {
                    auto rendererUDN = getManagerEngineer()->getZoneManager()->getRendererUDNForZoneUDN(it.first);
                    auto mediaRenderer = getVirtualMediaRendererFromUDN(rendererUDN);
                    if (mediaRenderer)
                    {
                        std::string zonePlaylistId = Raumkernel::Manager::LISTID_ZONEIDENTIFIER + mediaRenderer->getUDN();
                        lastUpdateIdCur = getManagerEngineer()->getMediaListManager()->getLastUpdateIdForList(zonePlaylistId);                       
                        if (!lastUpdateIdCur.empty())
                            lastUpdateSum += std::stoull(lastUpdateIdCur);
                    }
                }
                lastUpdateIdSum = std::to_string(lastUpdateSum);
            }

            return lastUpdateIdSum;
        }


        void RequestActionReturnableLongPolling_GetZoneMediaList::addMediaListToJson(const std::string &_zoneUDN, std::vector<std::shared_ptr<Raumkernel::Media::Item::MediaItem>> &_mediaList, rapidjson::Writer<rapidjson::StringBuffer> &_jsonWriter)
        {          
            _jsonWriter.StartObject();
            _jsonWriter.Key("udn"); _jsonWriter.String(_zoneUDN.c_str());
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


        bool RequestActionReturnableLongPolling_GetZoneMediaList::executeActionLongPolling()
        {
            auto id = getOptionValue("id");   
            std::vector<std::shared_ptr<Raumkernel::Media::Item::MediaItem>> mediaList;

            rapidjson::StringBuffer jsonStringBuffer;
            rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonStringBuffer);

            jsonWriter.StartArray();


            // if we got an id we get the list 
            if (!id.empty())
            {
                auto mediaRenderer = getVirtualMediaRenderer(id);
                if (!mediaRenderer)
                {
                    logError("Room or Zone with ID: " + id + " not found!", CURRENT_FUNCTION);
                    return false;
                }                
                std::string zonePlaylistId = Raumkernel::Manager::LISTID_ZONEIDENTIFIER + mediaRenderer->getUDN();                                
                // zone playlists do not have to be extra read, they are always up to date! 
                // So we do only get a copy of the list with shared pointers to media items
                mediaList = managerEngineer->getMediaListManager()->getList(zonePlaylistId);                          

                // add the list items to the jsonWriter
                // we do not have to lock when we are reading the copied list of the media items because they are shared pointers 
                // and media items only will be created once and will never be updated!
                addMediaListToJson(mediaRenderer->getUDN(), mediaList, jsonWriter);
            }
            // if we have no id provided, then we get the playlist for all virtual renderers
            else
            {
                auto zoneInfoMap = getManagerEngineer()->getZoneManager()->getZoneInformationMap();
                for (auto it : zoneInfoMap)
                {
                    std::string zonePlaylistId = Raumkernel::Manager::LISTID_ZONEIDENTIFIER + it.second.UDN;                                     
                    mediaList = managerEngineer->getMediaListManager()->getList(zonePlaylistId);

                    addMediaListToJson(it.second.UDN, mediaList, jsonWriter);
                }
            }            
                                   
            jsonWriter.EndArray();

            setResponseData(jsonStringBuffer.GetString());

            return true;            
        }
    }
}

