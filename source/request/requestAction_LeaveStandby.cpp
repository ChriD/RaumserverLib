
#include <raumserver/request/requestAction_LeaveStandby.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_LeaveStandby::RequestAction_LeaveStandby(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_LEAVESTANDBY;
        }


        RequestAction_LeaveStandby::RequestAction_LeaveStandby(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_LEAVESTANDBY;
        }


        RequestAction_LeaveStandby::~RequestAction_LeaveStandby()
        {
        }


        bool RequestAction_LeaveStandby::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:
            // raumserver/controller/leaveStandby
            // raumserver/controller/leaveStandby?id=Schlafzimmer
            // raumserver/controller/leaveStandby?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88

            return isValid;
        }

       
        bool RequestAction_LeaveStandby::executeAction()
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
                    mediaRenderer->leaveStandby(sync);
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
                                mediaRenderer->leaveStandby(sync);
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
