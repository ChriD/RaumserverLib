
#include <raumserver/request/requestActionReturnableLP_GetZoneConfig.h>

namespace Raumserver
{
    namespace Request
    {

        RequestActionReturnableLongPolling_GetZoneConfig::RequestActionReturnableLongPolling_GetZoneConfig(std::string _url) : RequestActionReturnableLongPolling(_url)
        {
            action = RequestActionType::RAA_GETZONECONFIG;
        }


        RequestActionReturnableLongPolling_GetZoneConfig::RequestActionReturnableLongPolling_GetZoneConfig(std::string _path, std::string _query) : RequestActionReturnableLongPolling(_path, _query)
        {
            action = RequestActionType::RAA_GETZONECONFIG;
        }


        RequestActionReturnableLongPolling_GetZoneConfig::~RequestActionReturnableLongPolling_GetZoneConfig()
        {
        }


        bool RequestActionReturnableLongPolling_GetZoneConfig::isValid()
        {
            bool isValid = RequestActionReturnableLongPolling::isValid();
            return isValid;
        }


        std::string RequestActionReturnableLongPolling_GetZoneConfig::getLastUpdateId()
        {
            //getManagerEngineer()->getDeviceManager()->lock();
            //getManagerEngineer()->getZoneManager()->lock();

            try
            {
                return managerEngineer->getZoneManager()->getLastUpdateId();
            }
            catch (...)
            {
                logError("Unknown error", CURRENT_POSITION);
            }

            //getManagerEngineer()->getZoneManager()->unlock();
            //getManagerEngineer()->getDeviceManager()->unlock();
        }


        bool RequestActionReturnableLongPolling_GetZoneConfig::executeActionLongPolling()
        {                            
            std::unordered_map<std::string, Raumkernel::Manager::ZoneInformation> zoneInfoMap;
            std::unordered_map<std::string, Raumkernel::Manager::RoomInformation> roomInfoMap;

            getManagerEngineer()->getDeviceManager()->lock();
            getManagerEngineer()->getZoneManager()->lock();

            try
            {
                zoneInfoMap = managerEngineer->getZoneManager()->getZoneInformationMap();
                roomInfoMap = managerEngineer->getZoneManager()->getRoomInformationMap();              
            }
            catch (...)
            {
                logError("Unknown error", CURRENT_POSITION);
            }                    

            getManagerEngineer()->getZoneManager()->unlock();
            getManagerEngineer()->getDeviceManager()->unlock(); 

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
            
            // add unasigned rooms to empty zone array object

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
        }
    }
}

