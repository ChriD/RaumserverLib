
#include <raumserver/request/requestAction_LoadContainer.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_LoadContainer::RequestAction_LoadContainer(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_LOADCONTAINER;
        }


        RequestAction_LoadContainer::RequestAction_LoadContainer(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_LOADCONTAINER;
        }


        RequestAction_LoadContainer::~RequestAction_LoadContainer()
        {
        }


        bool RequestAction_LoadContainer::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:            
            // raumserver/controller/loadcontainer?id=Schlafzimmer&value=[urlencoded container id]
            // raumserver/controller/loadcontainer?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88&value=[urlencoded container id]

            auto id = getOptionValue("id");
            auto value = getOptionValue("value");     

            if (id.empty())
            {
                logError("'id' option is needed to execute 'loadContainer' command!", CURRENT_FUNCTION);
                isValid = false;
            }
            if (value.empty())
            {
                logError("'value' option is needed to execute 'loadContainer' command!", CURRENT_FUNCTION);
                isValid = false;
            }

            return isValid;
        }


        bool RequestAction_LoadContainer::executeAction()
        {
            auto id = getOptionValue("id");
            auto value = getOptionValue("value");
            auto trackIndexString = getOptionValue("trackIndex");
            std::int32_t trackIndex = 0;

            if (!trackIndexString.empty())
                trackIndex = Raumkernel::Tools::CommonUtil::toInt32(trackIndexString);

            if (trackIndex < 0)
                trackIndex = 0;

            // if we got an id we try to stop the playing for the id (which may be a roomUDN, a zoneUDM or a roomName)
            if (!id.empty())
            {
                auto mediaRenderer = getVirtualMediaRenderer(id);
                if (!mediaRenderer)
                {
                    logError("Room or Zone with ID: " + id + " not found!", CURRENT_FUNCTION);
                    return false;
                }
                mediaRenderer->loadContainer(value, trackIndex, sync);
            }          

            return true;
        }
    }
}
