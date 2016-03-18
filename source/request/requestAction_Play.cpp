
#include <raumserver/request/requestAction_Play.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_Play::RequestAction_Play(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_PLAY;
        }


        RequestAction_Play::RequestAction_Play(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_PLAY;
        }


        RequestAction_Play::~RequestAction_Play()
        {
        }


        bool RequestAction_Play::executeAction()
        {
            //
            return true;
        }
    }
}
