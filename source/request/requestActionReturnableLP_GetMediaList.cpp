
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
            return managerEngineer->getZoneManager()->getLastUpdateId();                        
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
            //auto containerId = getOptionValue("containerId");            

            connections.connect(managerEngineer->getMediaListManager()->sigMediaListDataChanged, this, &RequestActionReturnableLongPolling_GetMediaList::onMediaListDataChanged);
            
            managerEngineer->getMediaListManager()->loadMediaItemListByContainerId(id);

            while (!listRetrieved)
            {
                // TODO: @@@
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }

            auto mediaList = managerEngineer->getMediaListManager()->getList(id);

            rapidjson::StringBuffer jsonStringBuffer;
            rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonStringBuffer);

            jsonWriter.StartArray();
            for (auto mediaItem : mediaList)
            {
                // todo: get from Media object itself?!?!
                jsonWriter.StartObject();
                jsonWriter.Key("id"); jsonWriter.String(mediaItem->id.c_str());
                jsonWriter.Key("parentId"); jsonWriter.String(mediaItem->parentId.c_str());
                jsonWriter.Key("id"); jsonWriter.String(mediaItem->raumfeldName.c_str());
                jsonWriter.EndObject();
            }
            jsonWriter.EndArray();

            setResponseData(jsonStringBuffer.GetString());


            // TODO: @@@
            // get list either from cache or retrieve new list (use the mediaListManager)
            // subscript to list changed/retreived event on mediaListManager. if there is the list with the given id then go on
            // check the updateIds of each list on the  mediaListManager manager


            return true;
            /*
            std::unordered_map<std::string, Raumkernel::Manager::ZoneInformation> zoneInfoMap;
            std::unordered_map<std::string, Raumkernel::Manager::RoomInformation> roomInfoMap;

            getManagerEngineer()->getDeviceManager()->lockDeviceList();
            getManagerEngineer()->getZoneManager()->lockLists();

            try
            {
                zoneInfoMap = managerEngineer->getZoneManager()->getZoneInformationMap();
                roomInfoMap = managerEngineer->getZoneManager()->getRoomInformationMap();              
            }
            catch (...)
            {
                logError("Unknown error", CURRENT_POSITION);
            }                    

            getManagerEngineer()->getZoneManager()->unlockLists();
            getManagerEngineer()->getDeviceManager()->unlockDeviceList(); 

            rapidjson::StringBuffer jsonStringBuffer;
            rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonStringBuffer);
                      
            jsonWriter.StartArray();
            

            for (auto pair : zoneInfoMap)
            {
                jsonWriter.StartObject();
                jsonWriter.Key("UDN"); jsonWriter.String(pair.first.c_str());
                jsonWriter.Key("name"); jsonWriter.String(pair.second.name.c_str());    
                jsonWriter.Key("rooms");
                jsonWriter.StartArray();

                for (auto roomUDN : pair.second.roomsUDN)
                {                    
                    jsonWriter.StartObject();

                    jsonWriter.Key("UDN"); jsonWriter.String(roomUDN.c_str());
                    
                    for (auto roomPair : roomInfoMap)
                    {
                        if (roomPair.first == roomUDN)
                        {                          
                            jsonWriter.Key("name"); jsonWriter.String(roomPair.second.name.c_str());
                            jsonWriter.Key("color"); jsonWriter.String(roomPair.second.color.c_str());
                            jsonWriter.Key("online"); jsonWriter.Bool(roomPair.second.isOnline);
                        }
                    }
                    jsonWriter.EndObject();                    
                }

                jsonWriter.EndArray();
                jsonWriter.EndObject();
            }                       
            
            // add unasigned rooms to emoty zone array object

            jsonWriter.StartObject();
            jsonWriter.Key("UDN"); jsonWriter.String("");
            jsonWriter.Key("name"); jsonWriter.String("");
            jsonWriter.Key("rooms");
            jsonWriter.StartArray();

            for (auto pair : roomInfoMap)
            {
                if (pair.second.zoneUDN.empty())
                {
                    jsonWriter.StartObject();
                    jsonWriter.Key("UDN"); jsonWriter.String(pair.second.UDN.c_str());
                    jsonWriter.Key("name"); jsonWriter.String(pair.second.name.c_str());
                    jsonWriter.Key("color"); jsonWriter.String(pair.second.color.c_str());
                    jsonWriter.Key("online"); jsonWriter.Bool(pair.second.isOnline);                                                   
                    jsonWriter.EndObject();                
                }
            }
            jsonWriter.EndArray();
            jsonWriter.EndObject();

            jsonWriter.EndArray();
                     
            setResponseData(jsonStringBuffer.GetString());

            return true;
            */
        }
    }
}

