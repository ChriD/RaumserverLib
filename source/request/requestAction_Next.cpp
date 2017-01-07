
#include <raumserver/request/requestAction_Next.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_Next::RequestAction_Next(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_NEXT;
        }


        RequestAction_Next::RequestAction_Next(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_NEXT;
        }


        RequestAction_Next::~RequestAction_Next()
        {
        }


        bool RequestAction_Next::isValid()
        {
            bool isValid = RequestAction::isValid();
           
            // examples for valid requests:            
            // raumserver/controller/next?id=Schlafzimmer
            // raumserver/controller/next?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88

            auto id = getOptionValue("id");
            if (id.empty())
            {
                logError("'id' option is needed to execute 'next' command!", CURRENT_FUNCTION);
                isValid = false;
            }

            return isValid;
        }


        bool RequestAction_Next::executeAction()
        {
            auto id = getOptionValue("id");

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
                    mediaRenderer->next(sync);
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
