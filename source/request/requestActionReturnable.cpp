
#include <raumserver/request/requestActionReturnable.h>

namespace Raumserver
{
    namespace Request
    {

        RequestActionReturnable::RequestActionReturnable(std::string _url) : RequestAction(_url)
        {      
            responseData = "";
        }


        RequestActionReturnable::RequestActionReturnable(std::string _path, std::string _query) : RequestAction(_path, _query)
        {     
            responseData = "";
        }


        RequestActionReturnable::~RequestActionReturnable()
        {
        }

     
        bool RequestActionReturnable::isAsyncExecutionAllowed()
        {
            return false;
        }

        bool RequestActionReturnable::isStackable()
        {
            return false;
        }


        std::string RequestActionReturnable::getResponseData()
        {
            return responseData;
        }


        void RequestActionReturnable::setResponseData(const std::string &_data)
        {
            responseData = _data;
        }
       
    }
}

