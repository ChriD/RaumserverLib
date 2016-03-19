
#include <raumserver/request/requestAction_AddToZone.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_AddToZone::RequestAction_AddToZone(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_ADDTOZONE;
        }


        RequestAction_AddToZone::RequestAction_AddToZone(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_ADDTOZONE;
        }


        RequestAction_AddToZone::~RequestAction_AddToZone()
        {
        }


        bool RequestAction_AddToZone::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:            
            // raumserver/controller/addToZone?id=Schlafzimmer&zoneid=Badezimmer
            // raumserver/controller/addToZone?id=Schlafzimmer,Wohnzimmer&zoneid=Badezimmer
            // raumserver/controller/addToZone?id=Schlafzimmer,Wohnzimmer&zoneid=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88
            // raumserver/controller/addToZone?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88&zoneid=Badezimmer

            return isValid;
        }

       
        bool RequestAction_AddToZone::executeAction()
        {
            auto id = getOptionValue("id");
                        
            if (!id.empty())
            {               
                // TODO: @@@
            }            

            return true;
        }      
    }
}
