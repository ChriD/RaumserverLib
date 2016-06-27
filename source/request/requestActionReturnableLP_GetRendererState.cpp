
#include <raumserver/request/requestActionReturnableLP_GetRendererState.h>
#include <raumserver/json/mediaItemJsonCreator.h>

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
            std::string lastUpdateIdCur, rendererUDN, lastUpdateIdSum;
            std::uint32_t lastUpdateSum = 0;

            // if we are called by a specific zone renderer id we only check this for a new update id
            auto id = getOptionValue("id");
            auto lpid = getOptionValue("updateId");

            if (!id.empty())
            {
                auto mediaRenderer = getVirtualMediaRenderer(id);
                if (!mediaRenderer)
                {
                    logError("Room or Zone with ID: " + id + " not found!", CURRENT_FUNCTION);
                    return "";
                }

                lastUpdateIdSum = mediaRenderer->getLastRendererStateUpdateId();
            }
            // run through the renderers and get current update id by summing up the values
            // if the value is other than given in header something has changed. 
            // It could be that the sum of the update ids may be the same (imaging bchanges on 2 renderers and the invidious case that the values would be the same sum) 
            else
            {
                auto zoneInfoMap = getManagerEngineer()->getZoneManager()->getZoneInformationMap();
                for (auto it : zoneInfoMap)
                {
                    auto rendererUDN = getManagerEngineer()->getZoneManager()->getRendererUDNForZoneUDN(it.first);
                    auto mediaRenderer = getVirtualMediaRendererFromUDN(rendererUDN);
                    if (mediaRenderer)
                    {
                        lastUpdateIdCur = mediaRenderer->getLastRendererStateUpdateId();                        
                        lastUpdateSum += std::stoi(lastUpdateIdCur);
                    }
                }
                lastUpdateIdSum = std::to_string(lastUpdateSum);
            }

            return lastUpdateIdSum;
        }


        void RequestActionReturnableLongPolling_GetRendererState::addRendererStateToJson(const std::string &_UDN, Raumkernel::Devices::MediaRendererState &_rendererState, std::shared_ptr<Raumkernel::Devices::MediaRenderer> _mediaRenderer, rapidjson::Writer<rapidjson::StringBuffer> &_jsonWriter)
        {
            // TODO: @@@
            _jsonWriter.StartObject();
            _jsonWriter.Key("udn"); _jsonWriter.String(_UDN.c_str());            
            _jsonWriter.Key("friendlyName"); _jsonWriter.String(_mediaRenderer->getFriendlyName().c_str());
            _jsonWriter.Key("isZoneRenderer"); _jsonWriter.Bool(_mediaRenderer->isZoneRenderer());
            _jsonWriter.Key("avTransportUri"); _jsonWriter.String(Raumkernel::Tools::UriUtil::unescape(_rendererState.aVTransportURI).c_str());
            _jsonWriter.Key("bitrate"); _jsonWriter.Uint(_rendererState.bitrate);
            _jsonWriter.Key("volume"); _jsonWriter.Uint(_rendererState.volume);
            _jsonWriter.Key("numberOfTracks"); _jsonWriter.Uint(_rendererState.numberOfTracks);
            _jsonWriter.Key("currentTrack"); _jsonWriter.Uint(_rendererState.currentTrack);            
            _jsonWriter.Key("currentTrackDuration"); _jsonWriter.Uint(_rendererState.currentTrackDuration);                        
            _jsonWriter.Key("muteState");_jsonWriter.String(Raumkernel::Devices::ConversionTool::muteStateToString(_rendererState.muteState).c_str());
            _jsonWriter.Key("playMode"); _jsonWriter.String(Raumkernel::Devices::ConversionTool::playModeToString(_rendererState.playMode).c_str());
            _jsonWriter.Key("transportState"); _jsonWriter.String(Raumkernel::Devices::ConversionTool::transportStateToString(_rendererState.transportState).c_str());

            _jsonWriter.Key("mediaItem");
            _jsonWriter.StartObject();            
            if (_rendererState.currentMediaItem)
            {                
                MediaItemJsonCreator::addJson(_rendererState.currentMediaItem, _jsonWriter);
            }
            _jsonWriter.EndObject();
                          

            _jsonWriter.Key("roomStates");
            _jsonWriter.StartArray();
            for (auto roomState : _rendererState.roomStates)
            {                
                _jsonWriter.StartObject();
                _jsonWriter.Key("roomUdn"); _jsonWriter.String(roomState.second.roomUDN.c_str());
                _jsonWriter.Key("isMute"); _jsonWriter.Bool(roomState.second.mute);
                _jsonWriter.Key("isOnline"); _jsonWriter.Bool(roomState.second.online);
                _jsonWriter.Key("volume"); _jsonWriter.Uint(roomState.second.volume);                
                _jsonWriter.Key("transportState"); _jsonWriter.String(Raumkernel::Devices::ConversionTool::transportStateToString(roomState.second.transportState).c_str());
                _jsonWriter.EndObject();
            }
            _jsonWriter.EndArray();
            _jsonWriter.EndObject();
        }


        bool RequestActionReturnableLongPolling_GetRendererState::executeActionLongPolling()
        {
            Raumkernel::Devices::MediaRendererState rendererState;

            auto id = getOptionValue("id");
            auto listAllStr = getOptionValue("listAll");            
            bool listAll = (listAllStr == "1" || listAllStr == "true") ? true : false;

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

                rendererState = mediaRenderer->state();
                addRendererStateToJson(mediaRenderer->getUDN(), rendererState, mediaRenderer, jsonWriter);
            }
            // if we have no id provided, then we get the renderer state for all virtual renderers
            // and for all other non virtual renderers
            else
            {
                // run through all virtual (zone) renderers
                auto zoneInfoMap = getManagerEngineer()->getZoneManager()->getZoneInformationMap();

                for (auto it : zoneInfoMap)
                {
                    auto rendererUDN = getManagerEngineer()->getZoneManager()->getRendererUDNForZoneUDN(it.first);
                    auto mediaRenderer = getVirtualMediaRendererFromUDN(rendererUDN);
                    if (mediaRenderer)
                    {
                        rendererState = mediaRenderer->state();
                        addRendererStateToJson(mediaRenderer->getUDN(), rendererState, mediaRenderer, jsonWriter);
                    }
                }

                // run through all renderers which are not in a zone
                // the 'empty' udn of the zone map tells us that this is the 'rooms without zone' gathering zone
                if (listAll)
                {
                    auto roomInfoMap = getManagerEngineer()->getZoneManager()->getRoomInformationMap();

                    for (auto pair : roomInfoMap)
                    {
                        if (pair.second.zoneUDN.empty())
                        {
                            for (auto rendererUDN : pair.second.rendererUDN)
                            {                                
                                auto mediaRenderer = getMediaRenderer(rendererUDN);
                                if (mediaRenderer)
                                {
                                    rendererState = mediaRenderer->state();                                    
                                    addRendererStateToJson(mediaRenderer->getUDN(), rendererState, mediaRenderer, jsonWriter);
                                }
                            }
                        }
                    }
                }

            }

            jsonWriter.EndArray();

            setResponseData(jsonStringBuffer.GetString());

            return true;
        }
    }
}

