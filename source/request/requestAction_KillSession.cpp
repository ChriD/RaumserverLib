
#include <raumserver/request/requestAction_KillSession.h>
#include <raumserver/manager/managerEngineerServer.h>

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
            auto sessionId = getOptionValue("sessionId");
                        
            // write the session id into a global session storage for killing
            // the manager will take care of killing the open requests
            if (!sessionId.empty())
                getManagerEngineerServer()->getSessionManager()->abortSession(sessionId);
            
            return true;
        }      
    }
}
