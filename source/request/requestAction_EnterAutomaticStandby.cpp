
#include <raumserver/request/requestAction_EnterAutomaticStandby.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_EnterAutomaticStandby::RequestAction_EnterAutomaticStandby(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_ENTERAUTOMATICSTANDBY;
        }


        RequestAction_EnterAutomaticStandby::RequestAction_EnterAutomaticStandby(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_ENTERAUTOMATICSTANDBY;
        }


        RequestAction_EnterAutomaticStandby::~RequestAction_EnterAutomaticStandby()
        {
        }


        bool RequestAction_EnterAutomaticStandby::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:
            // raumserver/controller/stop
            // raumserver/controller/stop?id=Schlafzimmer
            // raumserver/controller/stop?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88

            return isValid;
        }

       
        bool RequestAction_EnterAutomaticStandby::executeAction()
        {
            auto id = getOptionValue("id");

            if (!id.empty())
            {
                auto mediaRenderer = std::dynamic_pointer_cast<Raumkernel::Devices::MediaRenderer_Raumfeld>(getMediaRenderer(id));
                if (!mediaRenderer)
                {
                    logError("Room with ID: " + id + " not found!", CURRENT_FUNCTION);
                    return false;
                }
                mediaRenderer->enterAutomaticStandby(sync);
            }
            else
            {
                auto mediaRendererMap = getManagerEngineer()->getDeviceManager()->getMediaRenderers();
                for (auto it : mediaRendererMap)
                {
                    if (!it.second->isZoneRenderer())
                    {
                        auto mediaRenderer = std::dynamic_pointer_cast<Raumkernel::Devices::MediaRenderer_Raumfeld>(it.second);
                        if (mediaRenderer)
                            mediaRenderer->enterAutomaticStandby(sync);
                    }
                }
            }

            return true;
        }
    }
}
