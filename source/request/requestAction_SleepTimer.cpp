
#include <raumserver/request/requestAction_SleepTimer.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_SleepTimer::RequestAction_SleepTimer(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_SLEEPTIMER;
        }


        RequestAction_SleepTimer::RequestAction_SleepTimer(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_SLEEPTIMER;
        }


        RequestAction_SleepTimer::~RequestAction_SleepTimer()
        {
        }


        bool RequestAction_SleepTimer::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:
            // raumserver/controller/play
            // raumserver/controller/play?id=Schlafzimmer
            // raumserver/controller/play?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88

            return isValid;
        }


        bool RequestAction_SleepTimer::executeAction()
        {
            auto id = getOptionValue("id");

            // TODO: @@@

            return true;
        }
    }
}
