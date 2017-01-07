
#include <raumserver/request/requestAction_FadeToVolume.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_FadeToVolume::RequestAction_FadeToVolume(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_FADETOVOLUME;
        }


        RequestAction_FadeToVolume::RequestAction_FadeToVolume(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_FADETOVOLUME;
        }


        RequestAction_FadeToVolume::~RequestAction_FadeToVolume()
        {
        }


        bool RequestAction_FadeToVolume::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:
            // raumserver/controller/fadeToVolume?value=60
            // raumserver/controller/fadeToVolume?id=Schlafzimmer?value=60&duration=1000                    
            // raumserver/controller/fadeToVolume?id=Schlafzimmer?value=-10&relative=true   
            // raumserver/controller/fadeToVolume?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88?value=60   

            auto relativeValue = getOptionValue("relative");
            bool relative = (relativeValue == "true" || relativeValue == "1") ? true : false;

            auto value = getOptionValue("value");
            if (value.empty())
            {
                logError("'value' option is needed to execute 'fadeToVolume' command!", CURRENT_FUNCTION);
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


        bool RequestAction_FadeToVolume::executeAction()
        {
            auto id = getOptionValue("id");
            auto value = Raumkernel::Tools::StringUtil::tolower(getOptionValue("value"));
            auto valueVolume = Raumkernel::Tools::CommonUtil::toInt32(value);
            auto durationString = Raumkernel::Tools::StringUtil::tolower(getOptionValue("duration", "0"));
            auto duration = Raumkernel::Tools::CommonUtil::toInt32(durationString);
            auto relativeValue = getOptionValue("relative");
            bool relative = (relativeValue == "true" || relativeValue == "1") ? true : false;
            std::int32_t newVolumeValue = 0;            

            // set some standard duration if the value is not given!
            if (duration <= 0)
                duration = 2000;

            getManagerEngineer()->getDeviceManager()->lock();
            getManagerEngineer()->getZoneManager()->lock();

            try
            {
                // if we got an id we try to stop the playing for the id (which may be a roomUDN, a zoneUDM or a roomName)
                if (!id.empty())
                {
                    auto mediaRenderer = getVirtualMediaRenderer(id);
                    if (!mediaRenderer)
                    {
                        logError("Room or Zone with ID: " + id + " not found!", CURRENT_FUNCTION);
                        return false;
                    }

                    if (relative)
                        newVolumeValue = mediaRenderer->getVolume(true) + valueVolume;
                    else
                        newVolumeValue = valueVolume;
                    if (newVolumeValue > 100) newVolumeValue = 100;
                    if (newVolumeValue < 0) newVolumeValue = 0;
                                   
                    mediaRenderer->fadeToVolume(newVolumeValue, duration, sync);
                }
                // if we have no id provided, then we do the request an all zones
                else
                {
                    auto zoneInfoMap = getManagerEngineer()->getZoneManager()->getZoneInformationMap();
                    for (auto it : zoneInfoMap)
                    {
                        auto rendererUDN = getManagerEngineer()->getZoneManager()->getRendererUDNForZoneUDN(it.first);
                        auto mediaRenderer = getVirtualMediaRendererFromUDN(rendererUDN);
                        if (mediaRenderer)
                        {
                            if(relative)
                                newVolumeValue = mediaRenderer->getVolume(true) + valueVolume;
                            else
                                newVolumeValue = valueVolume;
                            if (newVolumeValue > 100) newVolumeValue = 100;
                            if (newVolumeValue < 0) newVolumeValue = 0;
                                         
                            mediaRenderer->fadeToVolume(newVolumeValue, duration, sync);
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
