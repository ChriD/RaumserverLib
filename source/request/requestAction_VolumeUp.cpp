
#include <raumserver/request/requestAction_VolumeUp.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_VolumeUp::RequestAction_VolumeUp(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_VOLUMEUP;
        }


        RequestAction_VolumeUp::RequestAction_VolumeUp(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_VOLUMEUP;
        }


        RequestAction_VolumeUp::~RequestAction_VolumeUp()
        {
        }


        bool RequestAction_VolumeUp::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:
            // raumserver/controller/volumeUp?value=2
            // raumserver/controller/volumeUp?id=Schlafzimmer&value=2
            // raumserver/controller/volumeUp?id=Schlafzimmer&value=2&scope=zone
            // raumserver/controller/volumeUp?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88&value=1
            // raumserver/controller/volumeUp?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88&value=1&scope=zone

            auto value = getOptionValue("value");
            if (value.empty())
            {
                logError("'value' option is needed to execute 'volumeUp' command!", CURRENT_FUNCTION);
                isValid = false;
            }

            auto valueChange = Raumkernel::Tools::CommonUtil::toInt32(value);
            if (valueChange <= 0)
            {
                logError("'value' has to be greater than '0'", CURRENT_FUNCTION);
                isValid = false;
            }

            return isValid;
        }


        bool RequestAction_VolumeUp::executeAction()
        {
            auto id = getOptionValue("id"); 
            auto scope = getOptionValue("scope");
            auto value = Raumkernel::Tools::StringUtil::tolower(getOptionValue("value"));
            auto valueChange = Raumkernel::Tools::CommonUtil::toInt32(value);
            auto zoneScope = isZoneScope(scope); 
            std::int32_t newVolumeValue = 0;

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
                    newVolumeValue = mediaRenderer->getVolume(true) + valueChange;
                    if (newVolumeValue > 100) newVolumeValue = 100;
                    if (newVolumeValue < 0) newVolumeValue = 0;
                    mediaRenderer->setVolume(newVolumeValue, sync);
                }
                else
                {
                    newVolumeValue = mediaRenderer->getRoomVolume(getRoomUDNFromId(id), true) + valueChange;
                    if (newVolumeValue > 100) newVolumeValue = 100;
                    if (newVolumeValue < 0) newVolumeValue = 0;
                    mediaRenderer->setRoomVolume(getRoomUDNFromId(id), newVolumeValue, sync);
                }
            }
            // if we have no id provided, we mute all renderers
            else
            {
                auto zoneInfoMap = getManagerEngineer()->getZoneManager()->getZoneInformationMap();
                for (auto it : zoneInfoMap)
                {
                    auto rendererUDN = getManagerEngineer()->getZoneManager()->getRendererUDNForZoneUDN(it.first);
                    auto mediaRenderer = std::dynamic_pointer_cast<Raumkernel::Devices::MediaRenderer_RaumfeldVirtual>(getManagerEngineer()->getDeviceManager()->getMediaRenderer(rendererUDN));
                    if (mediaRenderer)
                    {
                        newVolumeValue = mediaRenderer->getVolume(true) + valueChange;
                        if (newVolumeValue > 100) newVolumeValue = 100;
                        if (newVolumeValue < 0) newVolumeValue = 0;
                        mediaRenderer->setVolume(newVolumeValue, sync);
                    }
                }
            }

            return true;
        }
    }
}
