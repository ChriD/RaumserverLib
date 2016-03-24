
#include <raumserver/request/requestAction_AddToZone.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_AddToZone::RequestAction_AddToZone(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_ADDTOZONE;
        }


        RequestAction_AddToZone::RequestAction_AddToZone(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_ADDTOZONE;
        }


        RequestAction_AddToZone::~RequestAction_AddToZone()
        {
        }


        bool RequestAction_AddToZone::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:            
            // raumserver/controller/addToZone?id=Schlafzimmer&zoneid=Badezimmer
            // raumserver/controller/addToZone?id=Schlafzimmer,Wohnzimmer&zoneid=Badezimmer
            // raumserver/controller/addToZone?id=Schlafzimmer,Wohnzimmer&zoneid=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88
            // raumserver/controller/addToZone?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88&zoneid=Badezimmer

            auto id = getOptionValue("id");
            auto zoneId = getOptionValue("zoneid");
            if (id.empty() || zoneId.empty())
            {
                logError("'id' and 'zoneId' option is needed to execute 'addToZone' command!", CURRENT_FUNCTION);
                isValid = false;
            }
            return isValid;
        }

       
        bool RequestAction_AddToZone::executeAction()
        {
            std::uint16_t processTime = 0;
            std::vector<std::string> roomUDNs;
            auto id = getOptionValueMultiple("id");
            auto zoneId = getOptionValue("zoneid");
            bool allRoomsAdded = false;

            if (!id.empty())
            {
                for (auto it : id)
                {
                    auto roomUDN = getRoomUDNFromId(it);
                    if (getManagerEngineer()->getZoneManager()->existsRoomUDN(roomUDN))
                        roomUDNs.push_back(roomUDN);
                }

                auto zoneUDN = getZoneUDNFromId(zoneId);

                // no valid request if zone is not found and its defined!
                if (zoneUDN.empty() && !zoneId.empty())
                    return true;

                if (!roomUDNs.empty())
                {
                    // get a copy of the current roomInformation map (so we have the current ZoneUDN's where the rooms are in!)                  
                    //auto roomInfoMapOld = getManagerEngineer()->getZoneManager()->getRoomInformationMap();
                    // connect the rooms to the new zone
                    getManagerEngineer()->getZoneManager()->connectRoomsToZone(roomUDNs, zoneUDN);

                    // wait until all zones get the new UDN
                    if (sync)
                    {
                        // wait until room is added to a new zoneUDN or a timout happens                              
                        while (!allRoomsAdded && processTime <= timeout)
                        {                    
                            allRoomsAdded = true;
                            for (auto it : roomUDNs)
                            {                            
                                if (!getManagerEngineer()->getZoneManager()->isRoomInZone(it, zoneUDN))                               
                                    allRoomsAdded = false;                               
                            }

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
