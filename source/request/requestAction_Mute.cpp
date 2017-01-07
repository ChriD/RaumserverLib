
#include <raumserver/request/requestAction_Mute.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_Mute::RequestAction_Mute(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_MUTE;
        }


        RequestAction_Mute::RequestAction_Mute(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_MUTE;
        }


        RequestAction_Mute::~RequestAction_Mute()
        {
        }


        bool RequestAction_Mute::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:
            // raumserver/controller/mute
            // raumserver/controller/mute?id=Schlafzimmer
            // raumserver/controller/mute?id=Schlafzimmer&scope=zone
            // raumserver/controller/mute?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88
            // raumserver/controller/mute?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88&scope=zone

            return isValid;
        }


        bool RequestAction_Mute::executeAction()
        {
            auto id = getOptionValue("id"); 
            auto scope = getOptionValue("scope");
            auto value = Raumkernel::Tools::StringUtil::tolower(getOptionValue("value"));
            auto zoneScope = isZoneScope(scope);
            bool mute = (value == "true" || value == "1" || value.empty()) ? true : false;

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
                        mediaRenderer->setMute(true, sync);                
                    else                
                        mediaRenderer->setRoomMute(getRoomUDNFromId(id), mute, sync);
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
                            mediaRenderer->setMute(mute, sync);
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
