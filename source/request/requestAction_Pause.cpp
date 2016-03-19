
#include <raumserver/request/requestAction_Pause.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_Pause::RequestAction_Pause(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_PAUSE;
        }


        RequestAction_Pause::RequestAction_Pause(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_PAUSE;
        }


        RequestAction_Pause::~RequestAction_Pause()
        {
        }


        bool RequestAction_Pause::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:
            // raumserver/controller/pause
            // raumserver/controller/pause?id=Schlafzimmer
            // raumserver/controller/pause?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88

            return isValid;
        }


        bool RequestAction_Pause::executeAction()
        {
            auto id = getOptionValue("id");

            // if we got an id we try to pause the playing for the id (which may be a roomUDN, a zoneUDM or a roomName)
            if (!id.empty())
            {
                auto mediaRenderer = getVirtualMediaRenderer(id);
                if (!mediaRenderer)
                {
                    logError("Room or Zone with ID: " + id + " not found!", CURRENT_FUNCTION);
                    return false;
                }
                mediaRenderer->pause(sync);
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
                        mediaRenderer->pause(sync);
                }
            }

            return true;
        }
    }
}
