
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
            auto kernelVersion = getManagerEngineerServer()->getRequestActionManager()->getKernelVersion();
            auto serverVersion = getManagerEngineerServer()->getRequestActionManager()->getServerVersion();

            rapidjson::StringBuffer jsonStringBuffer;
            rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonStringBuffer);

            jsonWriter.StartObject();
            jsonWriter.Key("raumkernelLib"); jsonWriter.String(kernelVersion.appVersion.c_str());
            jsonWriter.Key("raumserverLib"); jsonWriter.String(serverVersion.appVersion.c_str());            
            jsonWriter.EndObject();           

            setResponseData(jsonStringBuffer.GetString());
           
            return true;
        }
    }
}

