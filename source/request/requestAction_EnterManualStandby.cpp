
#include <raumserver/request/requestAction_EnterManualStandby.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_EnterManualStandby::RequestAction_EnterManualStandby(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_ENTERMANUALSTANDBY;
        }


        RequestAction_EnterManualStandby::RequestAction_EnterManualStandby(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_ENTERMANUALSTANDBY;
        }


        RequestAction_EnterManualStandby::~RequestAction_EnterManualStandby()
        {
        }


        bool RequestAction_EnterManualStandby::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:
            // raumserver/controller/enterManualStandby
            // raumserver/controller/enterManualStandby?id=Schlafzimmer
            // raumserver/controller/enterManualStandby?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88

            return isValid;
        }

       
        bool RequestAction_EnterManualStandby::executeAction()
        {
            auto id = getOptionValue("id");

            getManagerEngineer()->getDeviceManager()->lock();
            getManagerEngineer()->getZoneManager()->lock();

            try
            {
                if (!id.empty())
                {
                    auto mediaRenderer = dynamic_cast<Raumkernel::Devices::MediaRenderer_Raumfeld*>(getMediaRenderer(id));
                    if (!mediaRenderer)
                    {
                        logError("Room with ID: " + id + " not found!", CURRENT_FUNCTION);
                        return false;
                    }
                    mediaRenderer->enterManualStandby(sync);
                }
                else
                {
                    auto mediaRendererMap = getManagerEngineer()->getDeviceManager()->getMediaRenderers();
                    for (auto it : mediaRendererMap)
                    {
                        if (!it.second->isZoneRenderer())
                        {
                            auto mediaRenderer = dynamic_cast<Raumkernel::Devices::MediaRenderer_Raumfeld*>(it.second);
                            if (mediaRenderer)
                                mediaRenderer->enterManualStandby(sync);
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
