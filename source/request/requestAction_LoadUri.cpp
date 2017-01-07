
#include <raumserver/request/requestAction_LoadUri.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_LoadUri::RequestAction_LoadUri(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_LOADURI;
        }


        RequestAction_LoadUri::RequestAction_LoadUri(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_LOADURI;
        }


        RequestAction_LoadUri::~RequestAction_LoadUri()
        {
        }


        bool RequestAction_LoadUri::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:            
            // raumserver/controller/loaduri?id=Schlafzimmer&value=[urlencoded uri]
            // raumserver/controller/loaduri?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88&value=[urlencoded uri]

            auto id = getOptionValue("id");
            auto value = getOptionValue("value");

            if (id.empty())
            {
                logError("'id' option is needed to execute 'loadUri' command!", CURRENT_FUNCTION);
                isValid = false;
            }
            if (value.empty())
            {
                logError("'value' option is needed to execute 'loadUri' command!", CURRENT_FUNCTION);
                isValid = false;
            }

            return isValid;
        }


        bool RequestAction_LoadUri::executeAction()
        {
            auto id = getOptionValue("id");
            auto value = getOptionValue("value");

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
                    mediaRenderer->loadUri(value, sync);
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
