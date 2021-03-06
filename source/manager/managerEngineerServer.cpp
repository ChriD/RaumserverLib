
#include <raumserver/manager/managerEngineerServer.h>
#include <signal.h>

namespace Raumserver
{
    namespace Manager
    {
        ManagerEngineerServer::ManagerEngineerServer() : RaumserverBase()
        {
            requestActionManager = nullptr;    
            systemReady = false;
        }


        ManagerEngineerServer::~ManagerEngineerServer()
        {
        }


        void ManagerEngineerServer::setSystemReady(bool _isReady)
        {
            systemReady = _isReady;
        }


        bool ManagerEngineerServer::isSystemReady()
        {
            return systemReady;
        }


        void ManagerEngineerServer::createManagers()
        {
            // create the settings manager wich will hold all of the settings for the kernel and the corresponding app
            logDebug("Create RequestActionManager-Manager...", CURRENT_FUNCTION);
            requestActionManager = std::shared_ptr<Manager::RequestActionManager>(new Manager::RequestActionManager());
            requestActionManager->setLogObject(getLogObject());

            logDebug("Create SessionManager-Manager...", CURRENT_FUNCTION);
            sessionManager = std::shared_ptr<Manager::SessionManager>(new Manager::SessionManager());
            sessionManager->setLogObject(getLogObject());
        }


        
        std::shared_ptr<RequestActionManager> ManagerEngineerServer::getRequestActionManager()
        {
            return requestActionManager;
        }


        std::shared_ptr<SessionManager> ManagerEngineerServer::getSessionManager()
        {
            return sessionManager;
        }


        void ManagerEngineerServer::raiseSigsegv()
        {
            raise(SIGSEGV);
        }
        
       

    }
}