
#include <raumserver/request/requestAction_Crash.h>
#include <raumserver/manager/managerEngineerServer.h>


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
                getManagerEngineerServer()->raiseSigsegv();
            }
            else
            {       
                crashLevel1();                
            }                
            return true;
        }


        void RequestAction_Crash::crashLevel1()
        {
            crashLevel2();
        }


        void RequestAction_Crash::crashLevel2()
        {
            crashLevel3();
        }

        void RequestAction_Crash::crashLevel3()
        {
            crashLevel4();
        }

        void RequestAction_Crash::crashLevel4()
        {
            getManagerEngineer()->raiseSigsegv();
        }


    }
}
