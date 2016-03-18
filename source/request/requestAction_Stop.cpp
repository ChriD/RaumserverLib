
#include <raumserver/request/requestAction_Stop.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_Stop::RequestAction_Stop(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_STOP;
        }


        RequestAction_Stop::RequestAction_Stop(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_STOP;
        }


        RequestAction_Stop::~RequestAction_Stop()
        {
        }


        bool RequestAction_Stop::isValid()
        {
            bool isValid = RequestAction::isValid();

            // raumserver/controller/stop?id=Schlafzimmer
            // raumserver/controller/stop?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88

            /*
            if (getOptionValue("id").empty())
            {
                logError("", CURRENT_FUNCTION);
                isValid = false;
            }*/

          
            // a stop action may onot nly be performed if we have a id. If there is no id all zones will be stopped            
            // We do only check if the options are provided, not if they are correct. This check will be done on execution itself

            // TODO: @@@

            return isValid;
        }

       
        bool RequestAction_Stop::executeAction()
        {
            auto id = getOptionValue("id");
            
            // if we got an id we try to stop the playing for the id (which may be a roomUDN, a zoneUDM or a roomName)
            if (!id.empty())
            {
                auto mediaRenderer = getVirtualMediaRenderer(id);
                if (!mediaRenderer)
                {
                    logError("Room or Zone with ID: " + id + " not found!", CURRENT_FUNCTION);
                    return false;
                }
                mediaRenderer->stop(sync);
                // TODO: we may wait until the stop has reached our kernel and we know it..?!?!?
            }
            // if we have no id provided, then  we stop all zones
            else
            {
                auto zoneInfoMap = getManagerEngineer()->getZoneManager()->getZoneInformationMap();
                for (auto it : zoneInfoMap)
                {
                    auto rendererUDN = getManagerEngineer()->getZoneManager()->getRendererUDNForZoneUDN(it.first);                    
                    auto mediaRenderer = std::dynamic_pointer_cast<Raumkernel::Devices::MediaRenderer_RaumfeldVirtual>(getManagerEngineer()->getDeviceManager()->getMediaRenderer(rendererUDN));
                    if (mediaRenderer)
                        mediaRenderer->stop(sync);                        
                }   
                // TODO: we may wait until the stop has reached our kernel and we know it..?!?!?
            }            

            return true;
        }      
    }
}
