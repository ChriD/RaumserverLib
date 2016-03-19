
#include <raumserver/request/requestAction_CreateZone.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_CreateZone::RequestAction_CreateZone(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_CREATEZONE;
        }


        RequestAction_CreateZone::RequestAction_CreateZone(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_CREATEZONE;
        }


        RequestAction_CreateZone::~RequestAction_CreateZone()
        {
        }


        bool RequestAction_CreateZone::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:            
            // raumserver/controller/createZone?id=Schlafzimmer
            // raumserver/controller/createZone?id=Schlafzimmer,Wohnzimmer 
            // raumserver/controller/createZone?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88

            return isValid;
        }

       
        bool RequestAction_CreateZone::executeAction()
        {
            auto id = getOptionValue("id");
                        
            if (!id.empty())
            {
                // TOSO: @@@
            }            

            return true;
        }      
    }
}
