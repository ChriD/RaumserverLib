
#include <raumserver/manager/sessionManager.h>

namespace Raumserver
{
    namespace Manager
    {

        SessionManager::SessionManager() : ManagerBaseServer()
        {                
        }

        SessionManager::~SessionManager()
        {               
        }

        
        void SessionManager::abortSession(std::string _sessionId)
        {
            std::unique_lock<std::mutex> lock(mutesSessionsToAbort);
            sessionsToAbort.push_back(_sessionId);
            // TODO: @@@
            // Well, following is only a workaround till i find a better solution.
            // if we have 8 sessions we did abort we do pop the oldest one (to free session ids)
            if (sessionsToAbort.size() >= 8)
                sessionsToAbort.erase(sessionsToAbort.begin());
        }

        
        bool SessionManager::isSessionAborted(std::string _sessionId)
        {            
            std::unique_lock<std::mutex> lock(mutesSessionsToAbort);
            if (std::find(sessionsToAbort.begin(), sessionsToAbort.end(), _sessionId) != sessionsToAbort.end())
            {
                return true;
            }
            return false;
        }
        
                    
    }
}