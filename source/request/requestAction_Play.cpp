
#include <raumserver/request/requestAction_Play.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_Play::RequestAction_Play(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_PLAY;
        }


        RequestAction_Play::RequestAction_Play(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_PLAY;
        }


        RequestAction_Play::~RequestAction_Play()
        {
        }


        bool RequestAction_Play::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:
            // raumserver/controller/play
            // raumserver/controller/play?id=Schlafzimmer
            // raumserver/controller/play?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88

            return isValid;
        }


        bool RequestAction_Play::executeAction()
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
                mediaRenderer->play(sync);
                // TODO: we may wait until the play has reached our kernel and we know it..?!?!?
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
                        mediaRenderer->play(sync);
                }
                // TODO: we may wait until the play has reached our kernel and we know it..?!?!?
            }

            return true;
        }
    }
}
