
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


        void RequestActionReturnableLongPolling_GetRendererState::addRendererStateToJson(const std::string &_UDN, Raumkernel::Devices::MediaRendererState &_rendererState, bool _isZoneRenderer, rapidjson::Writer<rapidjson::StringBuffer> &_jsonWriter)
        {
            // TODO: @@@
            _jsonWriter.StartObject();
            _jsonWriter.Key("udn"); _jsonWriter.String(_UDN.c_str());
            _jsonWriter.Key("isZoneRenderer"); _jsonWriter.Bool(_isZoneRenderer);            
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
            // TODO: add media item infos (get direct from media item?!)
            if (_rendererState.currentMediaItem)
            {
                _jsonWriter.Key("id"); _jsonWriter.String(_rendererState.currentMediaItem->id.c_str());
                _jsonWriter.Key("parentId"); _jsonWriter.String(_rendererState.currentMediaItem->parentId.c_str());
                _jsonWriter.Key("upnpClass"); _jsonWriter.String(_rendererState.currentMediaItem->upnpClass.c_str());
                _jsonWriter.Key("type"); _jsonWriter.String(Raumkernel::Media::Item::MediaItem::mediaItemTypeToString(_rendererState.currentMediaItem->type).c_str());
                _jsonWriter.Key("id"); _jsonWriter.String(_rendererState.currentMediaItem->raumfeldName.c_str());
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
                addRendererStateToJson(mediaRenderer->getUDN(), rendererState, jsonWriter);
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
                        addRendererStateToJson(mediaRenderer->getUDN(), rendererState, jsonWriter);
                    }
                }

                // run through all renderers which are not in a zone
                // the 'empty' udn of the zone map tells us that this is the 'rooms without zone' gathering zone
                if (listAll)
                {
                    for (auto it : zoneInfoMap)
                    {
                        if (it.first.empty())
                        {
                            for (auto roomUDN : it.second.roomsUDN)
                            {
                                auto rendererUDN = getManagerEngineer()->getZoneManager()->getRendererUDNForRoomUDN(roomUDN);
                                auto mediaRenderer = getMediaRenderer(rendererUDN);
                                if (mediaRenderer)
                                {
                                    rendererState = mediaRenderer->state();
                                    addRendererStateToJson(mediaRenderer->getUDN(), rendererState, jsonWriter);
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

