
#include <raumserver/request/requestActionReturnableLP.h>

namespace Raumserver
{
    namespace Request
    {

        RequestActionReturnableLongPolling::RequestActionReturnableLongPolling(std::string _url) : RequestActionReturnable(_url)
        {  
            action = RequestActionType::RAA_UNDEFINED;
        }


        RequestActionReturnableLongPolling::RequestActionReturnableLongPolling(std::string _path, std::string _query) : RequestActionReturnable(_path, _query)
        {    
            action = RequestActionType::RAA_UNDEFINED;
        }


        RequestActionReturnableLongPolling::~RequestActionReturnableLongPolling()
        {
        }
       

        bool RequestActionReturnableLongPolling::isValid()
        {
            bool isValid = RequestActionReturnable::isValid();  
            return isValid;
        }


        std::string RequestActionReturnableLongPolling::getLastUpdateId()
        {
            // this method has to be overwritten
            return "";
        }


        bool RequestActionReturnableLongPolling::executeActionLongPolling()
        {
            // this method has to be overwritten
            return false;
        }


        bool RequestActionReturnableLongPolling::executeAction()
        {          
            bool ret = false;
            std::string lastUpdateId = "";

            // get long polling id 
            std::string lpid = getOptionValue("updateId");

            // when there is no longpolling then only execute request
            if (lpid.empty())
            {
                lastUpdateId = getLastUpdateId();
                ret = executeActionLongPolling();
            }
            // if there is a long polling id we have to wait until the id changes before we execute the request
            else
            {
                while (true)
                {
                    lastUpdateId = getLastUpdateId();
                    if (lpid != lastUpdateId)
                    {
                        ret = executeActionLongPolling();
                        break;
                    }
                    // wait a little bit to keep cpu load and lockings low
                    std::this_thread::sleep_for(std::chrono::milliseconds(250));
                }
            }

            addResponseHeader("updateId", lastUpdateId);            
           
            return ret;
        }
    }
}

