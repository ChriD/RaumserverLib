
#include <raumserver/request/requestActionReturnable_GetVersion.h>
#include <raumserver/manager/managerEngineerServer.h>

namespace Raumserver
{
    namespace Request
    {

        RequestActionReturnable_GetVersion::RequestActionReturnable_GetVersion(std::string _url) : RequestActionReturnable(_url)
        {
            action = RequestActionType::RAA_GETVERSION;
        }


        RequestActionReturnable_GetVersion::RequestActionReturnable_GetVersion(std::string _path, std::string _query) : RequestActionReturnable(_path, _query)
        {
            action = RequestActionType::RAA_GETVERSION;
        }


        RequestActionReturnable_GetVersion::~RequestActionReturnable_GetVersion()
        {
        }
       

        bool RequestActionReturnable_GetVersion::isValid()
        {
            bool isValid = RequestAction::isValid();  
            return isValid;
        }


        bool RequestActionReturnable_GetVersion::executeAction()
        {          
            Json::Value versionInfo;

            auto kernelVersion = getManagerEngineerServer()->getRequestActionManager()->getKernelVersion();
            auto serverVersion = getManagerEngineerServer()->getRequestActionManager()->getServerVersion();
            
            versionInfo["versionInfo"]["raumkernelLib"] = kernelVersion.appVersion;
            versionInfo["versionInfo"]["raumserverLib"] = serverVersion.appVersion;

            setResponseData(versionInfo.toStyledString());
           
            return true;
        }
    }
}

