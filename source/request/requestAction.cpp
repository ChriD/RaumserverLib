
#include <raumserver/request/requestAction.h>
#include <raumserver/request/requestActions.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction::RequestAction(std::string _url) : RaumserverBaseMgr()
        {
            url = _url;
            query = "";
            sync = true;
        }

        RequestAction::RequestAction(std::string _path, std::string _query) : RaumserverBaseMgr()
        {
            url = _path;
            query = _query;
            sync = true;
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


        void RequestAction::parseQueryOptions()
        {
            if (query.empty())
                query = Raumkernel::Tools::UriUtil::getQueryFromUrl(url);
            requestOptions = Raumkernel::Tools::UriUtil::parseQueryString(query);
        }


        bool RequestAction::isValid()
        {       
            // try to parse the query options to the options map for further use
            parseQueryOptions();

            if (!isAsyncExecutionAllowed() && !sync)
                return false;
            return true;
        }


        std::string RequestAction::getOptionValue(std::string _key, std::string _default)
        {
            auto it = requestOptions.find(_key);
            if (it == requestOptions.end())
                return _default;            
            return it->second;
        }
          

        std::shared_ptr<Raumkernel::Devices::MediaRenderer_RaumfeldVirtual> RequestAction::getVirtualMediaRenderer(std::string _id)
        {
            if (!_id.empty())
            {
                std::string roomUDN, zoneUDN, rendererUDN;

                roomUDN = getManagerEngineer()->getZoneManager()->getRoomUDNForRoomName(_id);
                if (roomUDN.empty())                
                    roomUDN = _id;                                    
                zoneUDN = getManagerEngineer()->getZoneManager()->getZoneUDNForRoomUDN(roomUDN);
                if (zoneUDN.empty())                
                    zoneUDN = _id;                
                rendererUDN = getManagerEngineer()->getZoneManager()->getRendererUDNForZoneUDN(zoneUDN);                
                if (!rendererUDN.empty())
                    return std::dynamic_pointer_cast<Raumkernel::Devices::MediaRenderer_RaumfeldVirtual>(getManagerEngineer()->getDeviceManager()->getMediaRenderer(rendererUDN));
            }
            return nullptr;
        }


        void RequestAction::logError(std::string _log, std::string _location)
        {
            RaumserverBaseMgr::logError(_log, _location);
            if (!error.empty()) error += "\n";
            error += _log;
        }


        void RequestAction::logCritical(std::string _log, std::string _location)
        {
            RaumserverBaseMgr::logCritical(_log, _location);
            if (!error.empty()) error += "\n";
            error += _log;
        }


        bool RequestAction::execute()
        {            
            // check if the request is valid (mostly used for chekcing mandatory request options)
            if (isValid())
                return executeAction();
            else
            {                
                logError("Invalid request options! Please validate path and query keys and values!", CURRENT_FUNCTION);
            }
            return false;
        }


        bool RequestAction::executeAction()
        {
            // Overwrite!            
            return true;
        }


        std::string RequestAction::requestActionTypeToString(RequestActionType _requestActionType)
        {
            if (_requestActionType == RequestActionType::RAA_NEXT) return "NEXT";
            if (_requestActionType == RequestActionType::RAA_PAUSE) return "PAUSE";
            if (_requestActionType == RequestActionType::RAA_PLAY) return "PLAY";
            if (_requestActionType == RequestActionType::RAA_PREV) return "PREV";
            if (_requestActionType == RequestActionType::RAA_SETVOLUME) return "SETVOLUME";
            if (_requestActionType == RequestActionType::RAA_STOP) return "STOP";
            if (_requestActionType == RequestActionType::RAA_VOLUMEDOWN) return "VOLUMEDOWN";
            if (_requestActionType == RequestActionType::RAA_VOLUMEUP) return "VOLUMEUP";
            return "";
        }


        RequestActionType RequestAction::stringToRequestActionType(std::string _requestActionTypeString)
        {
            _requestActionTypeString = Raumkernel::Tools::StringUtil::toupper(_requestActionTypeString);
            if (_requestActionTypeString == "NEXT") return RequestActionType::RAA_NEXT;
            if (_requestActionTypeString == "PAUSE") return RequestActionType::RAA_PAUSE;
            if (_requestActionTypeString == "PLAY") return RequestActionType::RAA_PLAY;
            if (_requestActionTypeString == "PREV") return RequestActionType::RAA_PREV;
            if (_requestActionTypeString == "SETVOLUME") return RequestActionType::RAA_SETVOLUME;
            if (_requestActionTypeString == "STOP") return RequestActionType::RAA_STOP;
            if (_requestActionTypeString == "VOLUMEDOWN") return RequestActionType::RAA_VOLUMEDOWN;
            if (_requestActionTypeString == "VOLUMEUP") return RequestActionType::RAA_VOLUMEUP;  
            return RequestActionType::RAA_UNDEFINED;
        }


        std::string RequestAction::getRequestInfo()
        {
            std::string options;
            for (auto it : requestOptions)
            {
                if (!options.empty())
                    options += " | ";
                options += it.first + ": " + it.second;
            }
            return RequestAction::requestActionTypeToString(action) + " / " + options;
        }


        std::shared_ptr<RequestAction> RequestAction::createFromUrl(std::string _url)
        {
            auto url = LUrlParser::clParseURL::ParseURL(_url);
            return RequestAction::createFromPath(url.m_Path, url.m_Query);
        }


        std::shared_ptr<RequestAction> RequestAction::createFromPath(std::string _path, std::string _queryString)
        {          
            auto pathParts = Raumkernel::Tools::StringUtil::explodeString(_path, "/");
            // first path has to be empty, the second "raumserver" and the third path has to be "controller"
            // the third one should be the action. if there is another pathPart than we do have a problem
            if (pathParts.size() != 4)
                return nullptr;

            std::string action = pathParts[3];

            auto actionType = RequestAction::stringToRequestActionType(action);

            switch (actionType)
            {
                // TODO: @@@
                case RequestActionType::RAA_UNDEFINED: return nullptr;
                case RequestActionType::RAA_NEXT: return nullptr;
                case RequestActionType::RAA_PAUSE: return std::shared_ptr<RequestAction_Pause>(new RequestAction_Pause(_path, _queryString));
                case RequestActionType::RAA_PLAY: return std::shared_ptr<RequestAction_Play>(new RequestAction_Play(_path, _queryString));
                case RequestActionType::RAA_PREV: return nullptr;
                case RequestActionType::RAA_SETVOLUME: return nullptr;
                case RequestActionType::RAA_STOP: return std::shared_ptr<RequestAction_Stop>(new RequestAction_Stop(_path, _queryString));
                case RequestActionType::RAA_VOLUMEDOWN: return nullptr;
                case RequestActionType::RAA_VOLUMEUP: return nullptr;
            }
            

            return nullptr;
        }





        // ####################################

        // used for direct return such as a JSON Request aso...

        RequestActionReturnable::RequestActionReturnable(std::string _url) : RequestAction(_url)
        {
        }


        RequestActionReturnable::RequestActionReturnable(std::string _path, std::string _query) : RequestAction(_path, _query)
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
