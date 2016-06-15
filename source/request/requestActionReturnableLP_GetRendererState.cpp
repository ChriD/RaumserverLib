
#include <raumserver/request/requestActionReturnableLP_GetRendererState.h>

namespace Raumserver
{
    namespace Request
    {

        RequestActionReturnableLongPolling_GetRendererState::RequestActionReturnableLongPolling_GetRendererState(std::string _url) : RequestActionReturnableLongPolling(_url)
        {
            action = RequestActionType::RAA_GETRENDERERSTATE;
        }


        RequestActionReturnableLongPolling_GetRendererState::RequestActionReturnableLongPolling_GetRendererState(std::string _path, std::string _query) : RequestActionReturnableLongPolling(_path, _query)
        {
            action = RequestActionType::RAA_GETRENDERERSTATE;
        }


        RequestActionReturnableLongPolling_GetRendererState::~RequestActionReturnableLongPolling_GetRendererState()
        {
        }


        bool RequestActionReturnableLongPolling_GetRendererState::isValid()
        {
            bool isValid = RequestActionReturnableLongPolling::isValid();
            return isValid;
        }


        std::string RequestActionReturnableLongPolling_GetRendererState::getLastUpdateId()
        {
            return "";
        }


        bool RequestActionReturnableLongPolling_GetRendererState::executeActionLongPolling()
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
            // if we have no id provided, then we get the playlist for all virtual renderers
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

