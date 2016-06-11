
#include <raumserver/request/requestActionReturnableLP_GetZoneConfig.h>

namespace Raumserver
{
    namespace Request
    {

        RequestActionReturnableLongPolling_GetZoneConfig::RequestActionReturnableLongPolling_GetZoneConfig(std::string _url) : RequestActionReturnableLongPolling(_url)
        {
            action = RequestActionType::RAA_GETZONECONFIG;
        }


        RequestActionReturnableLongPolling_GetZoneConfig::RequestActionReturnableLongPolling_GetZoneConfig(std::string _path, std::string _query) : RequestActionReturnableLongPolling(_path, _query)
        {
            action = RequestActionType::RAA_GETZONECONFIG;
        }


        RequestActionReturnableLongPolling_GetZoneConfig::~RequestActionReturnableLongPolling_GetZoneConfig()
        {
        }


        bool RequestActionReturnableLongPolling_GetZoneConfig::isValid()
        {
            bool isValid = RequestActionReturnableLongPolling::isValid();
            return isValid;
        }


        std::string RequestActionReturnableLongPolling_GetZoneConfig::getActualLongPollingId()
        {
            // TODO: @@@
            return "";
        }


        bool RequestActionReturnableLongPolling_GetZoneConfig::executeActionLongPolling()
        {    
            //Json::Value root, versionInfo;

            getManagerEngineer()->getDeviceManager()->lockDeviceList();
            getManagerEngineer()->getZoneManager()->lockLists();

            // TODO: @@@

            getManagerEngineer()->getZoneManager()->unlockLists();
            getManagerEngineer()->getDeviceManager()->unlockDeviceList(); 


            //versionInfo["versionInfo"]["raumkernelLib"] = kernelVersion.appVersion;
            //versionInfo["versionInfo"]["raumserverLib"] = serverVersion.appVersion;

            //setResponseData(versionInfo.toStyledString());

            return true;
        }
    }
}

