
#include <raumserver/request/requestAction_ToggleMute.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_ToggleMute::RequestAction_ToggleMute(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_TOGGLEMUTE;
        }


        RequestAction_ToggleMute::RequestAction_ToggleMute(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_TOGGLEMUTE;
        }


        RequestAction_ToggleMute::~RequestAction_ToggleMute()
        {
        }


        bool RequestAction_ToggleMute::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:
            // raumserver/controller/togglemute
            // raumserver/controller/togglemute?id=Schlafzimmer
            // raumserver/controller/togglemute?id=Schlafzimmer&scope=zone
            // raumserver/controller/togglemute?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88
            // raumserver/controller/togglemute?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88&scope=zone

            return isValid;
        }


        bool RequestAction_ToggleMute::executeAction()
        {
            auto id = getOptionValue("id"); 
            auto scope = getOptionValue("scope");
            auto zoneScope = isZoneScope(scope);
            bool mute = false;

            getManagerEngineer()->getDeviceManager()->lock();
            getManagerEngineer()->getZoneManager()->lock();

            try
            {
                // we have got an id that might be a room or a zone. we have to get the scope to know what we should mute
                if (!id.empty())
                {
                    auto mediaRenderer = getVirtualMediaRenderer(id);
                    if (!mediaRenderer)
                    {
                        logError("Room or Zone with ID: " + id + " not found!", CURRENT_FUNCTION);
                        return false;
                    }
                    if (zoneScope)
                    {
                        mute = mediaRenderer->getMute(true);
                        mediaRenderer->setMute(!mute, sync);
                    }
                    else
                    {
                        mute = mediaRenderer->getRoomMute(getRoomUDNFromId(id), true);
                        mediaRenderer->setRoomMute(getRoomUDNFromId(id), !mute, sync);
                    }
                }
                // if we have no id provided, we mute all renderers
                else
                {
                    auto zoneInfoMap = getManagerEngineer()->getZoneManager()->getZoneInformationMap();
                    for (auto it : zoneInfoMap)
                    {
                        auto rendererUDN = getManagerEngineer()->getZoneManager()->getRendererUDNForZoneUDN(it.first);
                        auto mediaRenderer = getVirtualMediaRendererFromUDN(rendererUDN);
                        if (mediaRenderer)
                        {
                            mute = mediaRenderer->getMute(true);
                            mediaRenderer->setMute(!mute, sync);
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

            return true;
        }
    }
}
