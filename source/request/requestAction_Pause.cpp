
#include <raumserver/request/requestAction_Pause.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_Pause::RequestAction_Pause(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_PAUSE;
        }


        RequestAction_Pause::RequestAction_Pause(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_PAUSE;
        }


        RequestAction_Pause::~RequestAction_Pause()
        {
        }


        bool RequestAction_Pause::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:
            // raumserver/controller/pause
            // raumserver/controller/pause?id=Schlafzimmer
            // raumserver/controller/pause?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88

            return isValid;
        }


        bool RequestAction_Pause::executeAction()
        {
            //
            return true;
        }
    }
}
