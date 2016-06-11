
#include <raumserver/request/requestAction_SleepTimer.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_SleepTimer::RequestAction_SleepTimer(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_SLEEPTIMER;
        }


        RequestAction_SleepTimer::RequestAction_SleepTimer(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_SLEEPTIMER;
        }


        RequestAction_SleepTimer::~RequestAction_SleepTimer()
        {
        }


        bool RequestAction_SleepTimer::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:
            // raumserver/controller/sleepTimer&secondsUntilSleep=120&secondsForVolumeRamp=120
            // raumserver/controller/sleepTimer?id=Schlafzimmer&secondsUntilSleep=120&secondsForVolumeRamp=120
            // raumserver/controller/sleepTimer?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88&secondsUntilSleep=120&secondsForVolumeRamp=120

            auto secondsUntilSleepString = getOptionValue("secondsUntilSleep");
            auto secondsForVolumeRampString = getOptionValue("secondsForVolumeRamp", secondsUntilSleepString);

            if (secondsUntilSleepString.empty())
            {
                logError("'secondsUntilSleep' option is needed to execute 'sleepTimer' command!", CURRENT_FUNCTION);
                isValid = false;
            }

            auto secondsUntilSleep = Raumkernel::Tools::CommonUtil::toInt32(secondsUntilSleepString);
            auto secondsForVolumeRamp = Raumkernel::Tools::CommonUtil::toInt32(secondsForVolumeRampString);

            if (secondsUntilSleep < 50 && secondsUntilSleep != 0)
            {
                logError("'secondsUntilSleep' has to be greater than 50", CURRENT_FUNCTION);
                isValid = false;
            }

            if (secondsForVolumeRamp < 50 && secondsUntilSleep != 0)
            {
                logError("'secondsForVolumeRamp' has to be greater than 50", CURRENT_FUNCTION);
                isValid = false;
            }


            return isValid;
        }


        bool RequestAction_SleepTimer::executeAction()
        {
            auto id = getOptionValue("id");

            auto secondsUntilSleepString = getOptionValue("secondsUntilSleep");
            auto secondsForVolumeRampString = getOptionValue("secondsForVolumeRamp", secondsUntilSleepString);
            auto secondsUntilSleep = Raumkernel::Tools::CommonUtil::toInt32(secondsUntilSleepString);
            auto secondsForVolumeRamp = Raumkernel::Tools::CommonUtil::toInt32(secondsForVolumeRampString);          
         
            if (!id.empty())
            {
                auto mediaRenderer = getVirtualMediaRenderer(id);
                if (!mediaRenderer)
                {
                    logError("Room or Zone with ID: " + id + " not found!", CURRENT_FUNCTION);
                    return false;
                }
                if (secondsUntilSleep == 0)
                {                    
                    mediaRenderer->cancelSleepTimer();
                }
                else
                {
                    mediaRenderer->startSleepTimer(secondsUntilSleep, secondsForVolumeRamp);                    
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
                        if (secondsUntilSleep == 0)
                        {                                                        
                            mediaRenderer->cancelSleepTimer();
                        }
                        else
                        {
                            mediaRenderer->startSleepTimer(secondsUntilSleep, secondsForVolumeRamp);                            
                        }
                    }
                }
            }


            return true;
        }
    }
}
