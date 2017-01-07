
#include <raumserver/request/requestAction_CreateZone.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_CreateZone::RequestAction_CreateZone(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_CREATEZONE;
        }


        RequestAction_CreateZone::RequestAction_CreateZone(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_CREATEZONE;
        }


        RequestAction_CreateZone::~RequestAction_CreateZone()
        {
        }


        bool RequestAction_CreateZone::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:            
            // raumserver/controller/createZone?id=Schlafzimmer
            // raumserver/controller/createZone?id=Schlafzimmer,Wohnzimmer 
            // raumserver/controller/createZone?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88

            auto id = getOptionValue("id");
            if (id.empty())
            {
                logError("'id' option is needed to execute 'createZone' command!", CURRENT_FUNCTION);
                isValid = false;
            }

            return isValid;
        }

       
        bool RequestAction_CreateZone::executeAction()
        {
            std::uint16_t processTime = 0;
            std::vector<std::string> roomUDNs;
            auto id = getOptionValueMultiple("id");
            bool allRoomsAdded = false;

            if (!id.empty())
            {              
                getManagerEngineer()->getDeviceManager()->lock();
                getManagerEngineer()->getZoneManager()->lock();

                try
                {
                    for (auto it : id)
                    {
                        auto roomUDN = getRoomUDNFromId(it);
                        if (getManagerEngineer()->getZoneManager()->existsRoomUDN(roomUDN))
                            roomUDNs.push_back(roomUDN);
                    }           
                }
                catch (...)
                {
                    logError("Unknown Exception!", CURRENT_POSITION);
                }

                getManagerEngineer()->getDeviceManager()->unlock();
                getManagerEngineer()->getZoneManager()->unlock();

                if (!roomUDNs.empty())
                {
                    // get a copy of the current roomInformation map (so we have the current ZoneUDN's where the rooms are in!)                  
                    auto roomInfoMapOld = getManagerEngineer()->getZoneManager()->getRoomInformationMap();
                    // create the new zone with the given room UDNs
                    getManagerEngineer()->getZoneManager()->createZoneFromRooms(roomUDNs);
                    
                    // wait until all zones get a new UDN
                    if (sync)
                    {
                        // wait until room is added to a new zoneUDN or a timout happens                              
                        while (!allRoomsAdded && processTime <= timeout)
                        {
                            getManagerEngineer()->getDeviceManager()->lock();
                            getManagerEngineer()->getZoneManager()->lock();

                            try
                            {
                                // get the current infos of the room to check if zone has changed
                                auto roomInfoMapNew = getManagerEngineer()->getZoneManager()->getRoomInformationMap();
                                allRoomsAdded = true;

                                for (auto it : roomUDNs)
                                {
                                    auto oldRoomDataIt = roomInfoMapOld.find(it);
                                    auto newRoomDataIt = roomInfoMapNew.find(it);

                                    if (oldRoomDataIt != roomInfoMapOld.end() && newRoomDataIt != roomInfoMapNew.end())
                                    {
                                        if (oldRoomDataIt->second.zoneUDN != newRoomDataIt->second.zoneUDN && !newRoomDataIt->second.zoneUDN.empty())
                                        {
                                            // the room is in a new zone. Thats great, but we have to wait for the other rooms too
                                        }
                                        else
                                        {
                                            allRoomsAdded = false;
                                        }
                                    }
                                }
                            }
                            catch (...)
                            {
                                logError("Unknown Exception!", CURRENT_POSITION);
                            }

                            getManagerEngineer()->getDeviceManager()->unlock();
                            getManagerEngineer()->getZoneManager()->unlock();
                                
                            std::this_thread::sleep_for(std::chrono::milliseconds(waitTimeForRequestActionKernelResponse));
                            processTime += waitTimeForRequestActionKernelResponse;
                        }

                        if (processTime > timeout)
                            logWarning("Timout on request (" + std::to_string(timeout) + "): " + getRequestInfo(), CURRENT_FUNCTION);
                    }

                }                                                            
            }

            return true;
        }      
    }
}
