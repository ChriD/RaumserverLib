
#include <raumserver/request/requestAction_LoadPlaylist.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_LoadPlaylist::RequestAction_LoadPlaylist(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_LOADPLAYLIST;
        }


        RequestAction_LoadPlaylist::RequestAction_LoadPlaylist(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_LOADPLAYLIST;
        }


        RequestAction_LoadPlaylist::~RequestAction_LoadPlaylist()
        {
        }


        bool RequestAction_LoadPlaylist::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:            
            // raumserver/controller/loadPlaylist?id=Schlafzimmer&value=[urlencoded playlist id]
            // raumserver/controller/loadPlaylist?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88&value=[urlencoded playlist id]

            auto id = getOptionValue("id");
            auto value = getOptionValue("value");

            if (id.empty())
            {
                logError("'id' option is needed to execute 'loadPlaylist' command!", CURRENT_FUNCTION);
                isValid = false;
            }
            if (value.empty())
            {
                logError("'value' option is needed to execute 'loadPlaylist' command!", CURRENT_FUNCTION);
                isValid = false;
            }

            return isValid;
        }


        bool RequestAction_LoadPlaylist::executeAction()
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
                mediaRenderer->loadPlaylist(value, trackIndex, sync);
            }

            return true;
        }
    }
}
