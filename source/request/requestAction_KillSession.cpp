
#include <raumserver/request/requestAction_KillSession.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_KillSession::RequestAction_KillSession(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_KILLSESSION;
        }


        RequestAction_KillSession::RequestAction_KillSession(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_KILLSESSION;
        }


        RequestAction_KillSession::~RequestAction_KillSession()
        {
        }


        bool RequestAction_KillSession::isValid()
        {
            bool isValid = RequestAction::isValid();          
            return isValid;
        }

       
        bool RequestAction_KillSession::executeAction()
        {
            auto session = getOptionValue("session");
            
            // TODO: @@@
            
            return true;
        }      
    }
}
