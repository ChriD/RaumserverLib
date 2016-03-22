
#include <raumserver/request/requestAction_SetPlayMode.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_SetPlayMode::RequestAction_SetPlayMode(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_SETPLAYMODE;
        }


        RequestAction_SetPlayMode::RequestAction_SetPlayMode(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_SETPLAYMODE;
        }


        RequestAction_SetPlayMode::~RequestAction_SetPlayMode()
        {
        }


        bool RequestAction_SetPlayMode::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:
            // raumserver/controller/setPlayMode?mode=
            // raumserver/controller/setPlayMode?id=Schlafzimmer&mode=
            // raumserver/controller/setPlayMode?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88&mode=
            
            //valid play mode strings are: NORMAL, SHUFFLE, REPEAT_ONE, REPEAT_ALL, RANDOM, DIRECT_1, INTRO
                     
            auto playModeString = getOptionValue("mode");
            if (playModeString.empty())
            {
                logError("'mode' option is needed to execute 'setPlayMode' command!", CURRENT_FUNCTION);
                isValid = false;
            }

            auto playMode = Raumkernel::Devices::ConversionTool::stringToPlayMode(playModeString);
            if (playMode == Raumkernel::Devices::MediaRenderer_PlayMode::MRPLAYMODE_UNDEFINED)   
            {
                logError("'mode' option with value '" + playModeString + "' is wrong!", CURRENT_FUNCTION);
                isValid = false;
            }     

            return isValid;
        }


        bool RequestAction_SetPlayMode::executeAction()
        {
            auto id = getOptionValue("id");
            auto playModeString = getOptionValue("mode");
            auto playMode = Raumkernel::Devices::ConversionTool::stringToPlayMode(playModeString);

            if (!id.empty())
            {
                auto mediaRenderer = getVirtualMediaRenderer(id);
                if (!mediaRenderer)
                {
                    logError("Room or Zone with ID: " + id + " not found!", CURRENT_FUNCTION);
                    return false;
                }
                mediaRenderer->setPlayMode(playMode, sync);
            }
            else
            {
                auto zoneInfoMap = getManagerEngineer()->getZoneManager()->getZoneInformationMap();
                for (auto it : zoneInfoMap)
                {
                    auto rendererUDN = getManagerEngineer()->getZoneManager()->getRendererUDNForZoneUDN(it.first);
                    auto mediaRenderer = std::dynamic_pointer_cast<Raumkernel::Devices::MediaRenderer_RaumfeldVirtual>(getManagerEngineer()->getDeviceManager()->getMediaRenderer(rendererUDN));
                    if (mediaRenderer)
                        mediaRenderer->setPlayMode(playMode, sync);
                }
            }

            return true;
        }
    }
}
