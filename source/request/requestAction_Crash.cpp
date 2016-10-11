
#include <raumserver/request/requestAction_Crash.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_Crash::RequestAction_Crash(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_CRASH;
        }


        RequestAction_Crash::RequestAction_Crash(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_CRASH;
        }


        RequestAction_Crash::~RequestAction_Crash()
        {
        }


        bool RequestAction_Crash::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:
            // raumserver/controller/crash
            // raumserver/controller/crash?type=1            

            return isValid;
        }


        bool RequestAction_Crash::executeAction()
        {
            auto type = getOptionValue("type");

            if (type == "2")
            {                
                // TODO: crash kernel
            }
            else
            {
                // TODO: crash server
            }                
            return true;
        }
    }
}
