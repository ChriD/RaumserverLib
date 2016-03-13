
#include <raumserver/request/requestAction.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction::RequestAction() : RaumserverBaseMgr()
        {
        }


        RequestAction::~RequestAction()
        {
        }


        bool RequestAction::isStackable()
        {
            return true;
        }


        bool RequestAction::isAsyncExecutionAllowed()
        {
            return true;
        }


        void RequestAction::execute()
        {
            //
        }


        // ####################################

        RequestActionReturnable::RequestActionReturnable() : RequestAction()
        {
        }


        RequestActionReturnable::~RequestActionReturnable()
        {
        }

        bool RequestActionReturnable::isStackable()
        {
            return false;
        }


        bool RequestActionReturnable::isAsyncExecutionAllowed()
        {
            return false;
        }

    }
}
