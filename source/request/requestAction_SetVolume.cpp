
#include <raumserver/request/requestAction_SetVolume.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_SetVolume::RequestAction_SetVolume(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_SETVOLUME;
        }


        RequestAction_SetVolume::RequestAction_SetVolume(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_SETVOLUME;
        }


        RequestAction_SetVolume::~RequestAction_SetVolume()
        {
        }


        bool RequestAction_SetVolume::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:
            // raumserver/controller/setVolume?value=25
            // raumserver/controller/setVolume?id=Schlafzimmer&value=25
            // raumserver/controller/setVolume?id=Schlafzimmer&value=25&scope=zone
            // raumserver/controller/setVolume?id=Schlafzimmer&value=25&relative=true
            // raumserver/controller/setVolume?id=Schlafzimmer&value=25&scope=zone&relative=true
            // raumserver/controller/setVolume?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88&value=25
            // raumserver/controller/setVolume?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88&value=25&scope=zone

            auto relativeValue = getOptionValue("relative");
            bool relative = (relativeValue == "true" || relativeValue == "1") ? true : false;

            auto value = getOptionValue("value");
            if (value.empty())
            {
                logError("'value' option is needed to execute 'setVolume' command!", CURRENT_FUNCTION);
                isValid = false;
            }

            auto valueChange = Raumkernel::Tools::CommonUtil::toInt32(value);
            if ((valueChange < 0 || valueChange > 100) && relative == false)
            {
                logError("'value' has to be between 0 and 100", CURRENT_FUNCTION);
                isValid = false;
            }

            return isValid;
        }


        bool RequestAction_SetVolume::executeAction()
        {
            auto id = getOptionValue("id");
            auto scope = getOptionValue("scope");
            auto value = Raumkernel::Tools::StringUtil::tolower(getOptionValue("value"));
            auto valueVolume = Raumkernel::Tools::CommonUtil::toInt32(value);
            auto relativeValue = getOptionValue("relative");
            bool relative = (relativeValue == "true" || relativeValue == "1") ? true : false;
            auto zoneScope = isZoneScope(scope);
            std::int32_t newVolumeValue = 0;

            getManagerEngineer()->getDeviceManager()->lock();
            getManagerEngineer()->getZoneManager()->lock();

            try
            {
                // we have got an id that might be a room or a zone. we have to get the scope to know what we should set the volume
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
                        if (relative)
                            newVolumeValue = mediaRenderer->getVolume(true) + valueVolume;
                        else
                            newVolumeValue = valueVolume;

                        if (newVolumeValue > 100) newVolumeValue = 100;
                        if (newVolumeValue < 0) newVolumeValue = 0;
                
                        mediaRenderer->setVolume(newVolumeValue, sync);

                    }
                    else
                    {
                        if (relative)
                            newVolumeValue = mediaRenderer->getRoomVolume(getRoomUDNFromId(id), true) + valueVolume;
                        else
                            newVolumeValue = valueVolume;

                        if (newVolumeValue > 100) newVolumeValue = 100;
                        if (newVolumeValue < 0) newVolumeValue = 0;

                        mediaRenderer->setRoomVolume(getRoomUDNFromId(id), newVolumeValue, sync);
                    }
                }
                // if we have no id provided, we do action on all renderers
                else
                {
                    auto zoneInfoMap = getManagerEngineer()->getZoneManager()->getZoneInformationMap();
                    for (auto it : zoneInfoMap)
                    {
                        auto rendererUDN = getManagerEngineer()->getZoneManager()->getRendererUDNForZoneUDN(it.first);
                        auto mediaRenderer = getVirtualMediaRendererFromUDN(rendererUDN);
                        if (mediaRenderer)
                        {
                            if (relative)
                                newVolumeValue = mediaRenderer->getVolume(true) + valueVolume;
                            else
                                newVolumeValue = valueVolume;

                            if (newVolumeValue > 100) newVolumeValue = 100;
                            if (newVolumeValue < 0) newVolumeValue = 0;

                            mediaRenderer->setVolume(newVolumeValue, sync);
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
