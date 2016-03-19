
#include <raumserver/request/requestAction_Prev.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_Prev::RequestAction_Prev(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_PREV;
        }


        RequestAction_Prev::RequestAction_Prev(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_PREV;
        }


        RequestAction_Prev::~RequestAction_Prev()
        {
        }


        bool RequestAction_Prev::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:            
            // raumserver/controller/prev?id=Schlafzimmer
            // raumserver/controller/prev?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88

            auto id = getOptionValue("id");
            if (id.empty())
            {
                logError("'id' option is needed to execute 'next' command!", CURRENT_FUNCTION);
                isValid = false;
            }

            return isValid;
        }


        bool RequestAction_Prev::executeAction()
        {
            auto id = getOptionValue("id");

            // if we got an id we try to stop the playing for the id (which may be a roomUDN, a zoneUDM or a roomName)
            if (!id.empty())
            {
                auto mediaRenderer = getVirtualMediaRenderer(id);
                if (!mediaRenderer)
                {
                    logError("Room or Zone with ID: " + id + " not found!", CURRENT_FUNCTION);
                    return false;
                }
                mediaRenderer->previous(sync);             
            }           

            return true;
        }
    }
}
