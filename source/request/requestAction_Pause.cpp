
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


        bool RequestAction_Pause::executeAction()
        {
            //
            return true;
        }
    }
}
