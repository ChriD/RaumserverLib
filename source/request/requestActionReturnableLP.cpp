
#include <raumserver/request/requestActionReturnableLP.h>
#include <raumserver/manager/managerEngineerServer.h>

namespace Raumserver
{
    namespace Request
    {

        RequestActionReturnableLongPolling::RequestActionReturnableLongPolling(std::string _url) : RequestActionReturnable(_url)
        {  
            action = RequestActionType::RAA_UNDEFINED;
            lastUpdateId = "";
        }


        RequestActionReturnableLongPolling::RequestActionReturnableLongPolling(std::string _path, std::string _query) : RequestActionReturnable(_path, _query)
        {    
            action = RequestActionType::RAA_UNDEFINED;
            lastUpdateId = "";
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


        bool RequestActionReturnableLongPolling::hasLastUpdateIdChanged()
        {                        
            std::string lpid = getOptionValue("updateId");
            if (lpid != lastUpdateId)
                return true;
            return false;
        }


        bool RequestActionReturnableLongPolling::executeActionLongPolling()
        {
            // this method has to be overwritten
            return false;
        }


        bool RequestActionReturnableLongPolling::executeAction()
        {          
            bool ret = false;            
            
            auto lpid = getOptionValue("updateId");
            auto sessionId = getOptionValue("sessionId");       
            
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
                    if (hasLastUpdateIdChanged())
                    {
                        ret = executeActionLongPolling();
                        break;
                    }

                    // check if session was killed, if so then return from the request                   
                    if (!sessionId.empty() && getManagerEngineerServer()->getSessionManager()->isSessionAborted(sessionId))
                    {
                        ret = false;
                        break;
                    }                                    

                    // wait a little bit to keep cpu load and lockings low
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                }
            }

            addResponseHeader("updateId", lastUpdateId);            
           
            return ret;
        }
    }
}

