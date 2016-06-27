
#include <raumserver/request/requestActionReturnableLP_GetRendererTransportState.h>

namespace Raumserver
{
    namespace Request
    {

        RequestActionReturnableLongPolling_GetRendererTransportState::RequestActionReturnableLongPolling_GetRendererTransportState(std::string _url) : RequestActionReturnableLongPolling(_url)
        {
            action = RequestActionType::RAA_GETRENDERERTRANSPORTSTATE;
        }


        RequestActionReturnableLongPolling_GetRendererTransportState::RequestActionReturnableLongPolling_GetRendererTransportState(std::string _path, std::string _query) : RequestActionReturnableLongPolling(_path, _query)
        {
            action = RequestActionType::RAA_GETRENDERERTRANSPORTSTATE;
        }


        RequestActionReturnableLongPolling_GetRendererTransportState::~RequestActionReturnableLongPolling_GetRendererTransportState()
        {
        }


        bool RequestActionReturnableLongPolling_GetRendererTransportState::isValid()
        {
            bool isValid = RequestActionReturnableLongPolling::isValid();            
            return isValid;
        }


        std::string RequestActionReturnableLongPolling_GetRendererTransportState::getLastUpdateId()
        {           
            return "";
        }
       

        bool RequestActionReturnableLongPolling_GetRendererTransportState::executeActionLongPolling()
        {
            auto id = getOptionValue("id");   

            rapidjson::StringBuffer jsonStringBuffer;
            rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonStringBuffer);

            jsonWriter.StartArray();

            if (!id.empty())
            {
                auto mediaRenderer = getVirtualMediaRenderer(id);
                if (!mediaRenderer)
                {
                    logError("Room or Zone with ID: " + id + " not found!", CURRENT_FUNCTION);
                    return false;
                }                
               
                // TODO: @@@
            }           
            else
            {
                auto zoneInfoMap = getManagerEngineer()->getZoneManager()->getZoneInformationMap();
                for (auto it : zoneInfoMap)
                {
                    // TODO: @@@
                }
            }            
                                   
            jsonWriter.EndArray();

            setResponseData(jsonStringBuffer.GetString());

            return true;            
        }
    }
}

