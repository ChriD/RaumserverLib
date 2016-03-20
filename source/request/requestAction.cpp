
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
            error = "";
            waitTimeAfterExecution = 0;
            timeout = 5000;
            waitTimeForRequestActionKernelResponse = 25;
        }

        RequestAction::RequestAction(std::string _path, std::string _query) : RaumserverBaseMgr()
        {
            url = _path;
            query = _query;
            sync = true;
            error = "";
            waitTimeAfterExecution = 0;
            timeout = 5000;
            waitTimeForRequestActionKernelResponse = 25;
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
            _key = Raumkernel::Tools::StringUtil::tolower(_key);
            auto it = requestOptions.find(_key);
            if (it == requestOptions.end())
                return _default;            
            return it->second;
        }


        std::vector<std::string> RequestAction::getOptionValueMultiple(std::string _key, std::string _delimiter)
        {
            std::vector<std::string> values;
            auto optionValue = getOptionValue(_key, "");
            if (!optionValue.empty())
                values = Raumkernel::Tools::StringUtil::explodeString(optionValue, _delimiter);
            return values;
        }


        std::string RequestAction::getRoomUDNFromId(std::string _id)
        {
            auto roomUDN = getManagerEngineer()->getZoneManager()->getRoomUDNForRoomName(_id);
            if (roomUDN.empty())
                roomUDN = _id;
            // check if room UDN is valid, otherwise return empty string
            if (!getManagerEngineer()->getZoneManager()->existsRoomUDN(roomUDN))
            {
                logError("Room for ID '" + _id  + "' not found", CURRENT_FUNCTION);
                return "";
            }
            return roomUDN;
        }       


        std::string RequestAction::getZoneUDNFromId(std::string _id)
        {
            std::string zoneUDN = "", roomUDN = "";
            roomUDN = getRoomUDNFromId(_id);
            if (!roomUDN.empty())
                zoneUDN = getManagerEngineer()->getZoneManager()->getZoneUDNForRoomUDN(roomUDN);
            if (zoneUDN.empty())
                zoneUDN = _id;
            // check if zone UDN is valid, otherwise return an empty string
            if (!getManagerEngineer()->getZoneManager()->existsZoneUDN(zoneUDN))
            {
                logError("Zone for ID '" + _id + "' not found", CURRENT_FUNCTION);
                return "";
            }
            return zoneUDN;
        }
          

        std::shared_ptr<Raumkernel::Devices::MediaRenderer_RaumfeldVirtual> RequestAction::getVirtualMediaRenderer(std::string _id)
        {
            if (!_id.empty())
            {                
                std::string zoneUDN = "", rendererUDN = "";
                zoneUDN = getZoneUDNFromId(_id);                               
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
            bool ret = false;
            std::uint32_t waitTime = waitTimeAfterExecution;

            // check if the request is valid (mostly used for checking mandatory request options)
            if (isValid())
            {
                auto measurePoint1 = std::chrono::system_clock::now().time_since_epoch();
                   
                ret = executeAction();

                auto measurePoint2 = std::chrono::system_clock::now().time_since_epoch();

                // put out request process time information
                auto durationMS = std::chrono::duration_cast<std::chrono::milliseconds>(measurePoint2).count() - std::chrono::duration_cast<std::chrono::milliseconds>(measurePoint1).count();
                logDebug("Request duration: " + std::to_string(durationMS) + "ms: " + getRequestInfo(), CURRENT_FUNCTION);
                                 
                // after execution of the request there may be a wait time we have to wait. The wait time may be provided
                // by the query of the uri or its defined directly on the request object
                auto waitStr = getOptionValue("wait", "0");
                if (!waitStr.empty())                
                    waitTime = std::stoi(waitStr);
                if (waitTime)
                    std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
            }
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
            if (_requestActionType == RequestActionType::RAA_VOLUMECHANGE) return "VOLUMECHANGE";
            if (_requestActionType == RequestActionType::RAA_CREATEZONE) return "CREATEZONE";
            if (_requestActionType == RequestActionType::RAA_ADDTOZONE) return "ADDTOZONE";
            if (_requestActionType == RequestActionType::RAA_DROPFROMZONE) return "DROPFROMZONE";
            if (_requestActionType == RequestActionType::RAA_MUTE) return "MUTE";
            if (_requestActionType == RequestActionType::RAA_UNMUTE) return "UNMUTE";
            if (_requestActionType == RequestActionType::RAA_SETPLAYMODE) return "SETPLAYMODE";
            if (_requestActionType == RequestActionType::RAA_LOADPLAYLIST) return "LOADPLAYLIST";
            if (_requestActionType == RequestActionType::RAA_LOADCONTAINER) return "LOADCONTAINER";
            if (_requestActionType == RequestActionType::RAA_LOADURI) return "LOADURI";
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
            if (_requestActionTypeString == "VOLUMECHANGE") return RequestActionType::RAA_VOLUMECHANGE;
            if (_requestActionTypeString == "CREATEZONE") return RequestActionType::RAA_CREATEZONE;
            if (_requestActionTypeString == "ADDTOZONE") return RequestActionType::RAA_ADDTOZONE;
            if (_requestActionTypeString == "DROPFROMZONE") return RequestActionType::RAA_DROPFROMZONE;
            if (_requestActionTypeString == "MUTE") return RequestActionType::RAA_MUTE;
            if (_requestActionTypeString == "UNMUTE") return RequestActionType::RAA_UNMUTE;
            if (_requestActionTypeString == "SETPLAYMODE") return RequestActionType::RAA_SETPLAYMODE;
            if (_requestActionTypeString == "LOADPLAYLIST") return RequestActionType::RAA_LOADPLAYLIST;
            if (_requestActionTypeString == "LOADCONTAINER") return RequestActionType::RAA_LOADCONTAINER;
            if (_requestActionTypeString == "LOADURI") return RequestActionType::RAA_LOADURI;

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


        std::string RequestAction::getErrors()
        {
            return error;
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
                case RequestActionType::RAA_NEXT: return std::shared_ptr<RequestAction_Next>(new RequestAction_Next(_path, _queryString));;
                case RequestActionType::RAA_PAUSE: return std::shared_ptr<RequestAction_Pause>(new RequestAction_Pause(_path, _queryString));
                case RequestActionType::RAA_PLAY: return std::shared_ptr<RequestAction_Play>(new RequestAction_Play(_path, _queryString));
                case RequestActionType::RAA_PREV: return std::shared_ptr<RequestAction_Prev>(new RequestAction_Prev(_path, _queryString));;
                case RequestActionType::RAA_SETVOLUME: return nullptr;
                case RequestActionType::RAA_STOP: return std::shared_ptr<RequestAction_Stop>(new RequestAction_Stop(_path, _queryString));
                case RequestActionType::RAA_VOLUMEDOWN: return nullptr;
                case RequestActionType::RAA_VOLUMEUP: return nullptr;                    
                case RequestActionType::RAA_CREATEZONE: return std::shared_ptr<RequestAction_CreateZone>(new RequestAction_CreateZone(_path, _queryString));
                case RequestActionType::RAA_ADDTOZONE: return std::shared_ptr<RequestAction_AddToZone>(new RequestAction_AddToZone(_path, _queryString));
                case RequestActionType::RAA_DROPFROMZONE: return std::shared_ptr<RequestAction_DropFromZone>(new RequestAction_DropFromZone(_path, _queryString));
                case RequestActionType::RAA_MUTE: return nullptr;
                case RequestActionType::RAA_UNMUTE: return nullptr;
                case RequestActionType::RAA_SETPLAYMODE: return nullptr;
                case RequestActionType::RAA_LOADPLAYLIST: return nullptr;
                case RequestActionType::RAA_LOADCONTAINER: return nullptr;
                case RequestActionType::RAA_LOADURI: return nullptr;
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
