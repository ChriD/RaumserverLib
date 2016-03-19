
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

            // examples for valid requests:
            // raumserver/controller/stop
            // raumserver/controller/stop?id=Schlafzimmer
            // raumserver/controller/stop?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88

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
            }
            // if we have no id provided, then we stop all zones
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
            }            

            return true;
        }      
    }
}
