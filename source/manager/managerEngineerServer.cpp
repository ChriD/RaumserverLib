
#include <raumserver/manager/managerEngineerServer.h>

namespace Raumserver
{
    namespace Manager
    {
        ManagerEngineerServer::ManagerEngineerServer() : RaumserverBase()
        {
            requestActionManager = nullptr;          
        }


        ManagerEngineerServer::~ManagerEngineerServer()
        {
        }


        void ManagerEngineerServer::createManagers()
        {
            // create the settings manager wich will hold all of the settings for the kernel and the corresponding app
            logDebug("Create RequestActionManager-Manager...", CURRENT_FUNCTION);
            requestActionManager = std::shared_ptr<Manager::RequestActionManager>(new Manager::RequestActionManager());
            requestActionManager->setLogObject(getLogObject());
        }


        
        std::shared_ptr<RequestActionManager> ManagerEngineerServer::getRequestActionManager()
        {
            return requestActionManager;
        }
        
       

    }
}