//
// The MIT License (MIT)
//
// Copyright (c) 2015 by ChriD
//
// Permission is hereby granted, free of charge,  to any person obtaining a copy of
// this software and  associated documentation  files  (the "Software"), to deal in
// the  Software  without  restriction,  including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software,  and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this  permission notice  shall be included in all
// copies or substantial portions of the Software.
//
// THE  SOFTWARE  IS  PROVIDED  "AS IS",  WITHOUT  WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE  AND NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE  LIABLE FOR ANY CLAIM,  DAMAGES OR OTHER LIABILITY, WHETHER
// IN  AN  ACTION  OF  CONTRACT,  TORT  OR  OTHERWISE,  ARISING  FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#pragma once
#ifndef RAUMSERVER_REQUESTACTION_H
#define RAUMSERVER_REQUESTACTION_H

#include <chrono>
#include <raumserver/raumserverBaseMgr.h>
#include <raumkernel/manager/managerEngineer.h>
#include <raumkernel/manager/zoneManager.h>
#include <raumkernel/manager/deviceManager.h>

namespace Raumserver
{
    namespace Request
    {
        enum class RequestActionType { RAA_UNDEFINED, RAA_PLAY, RAA_PAUSE, RAA_STOP, RAA_NEXT, RAA_PREV, RAA_VOLUMECHANGE, RAA_VOLUMEUP, RAA_VOLUMEDOWN, RAA_SETVOLUME, 
                                       RAA_CREATEZONE, RAA_ADDTOZONE, RAA_DROPFROMZONE, RAA_MUTE, RAA_UNMUTE , RAA_SETPLAYMODE, RAA_LOADPLAYLIST, RAA_LOADCONTAINER, RAA_LOADURI, RAA_SEEK};
        enum class RequestReceiver { RR_ROOM, RR_ZONE, RR_JSON };
     
        class RequestAction : public RaumserverBaseMgr
        {
            public:
                EXPORT RequestAction(std::string _url);
                EXPORT RequestAction(std::string _path, std::string _query);
                EXPORT virtual ~RequestAction();

                /**
                * returns if the request action is stackable or not
                * if it's not stackable it has to be executed directly after the request was received
                * if a requestAction is stackable does not mean that it has to be called in sync!
                */
                EXPORT virtual bool isStackable();
                /**
                * returns if the request action is allowed to be called assyncron
                */
                EXPORT virtual bool isAsyncExecutionAllowed();
                /**
                * executes the given action either in a sync mode or async (as given in 'sync' var)
                * This method has to be overwritten 
                */
                EXPORT virtual bool execute();                
                /**
                * returns a string identification for the requestActionType enum
                */
                EXPORT static std::string requestActionTypeToString(RequestActionType _requestActionType);
                /**
                * returns a requestActionType enum fro a string
                */
                EXPORT static RequestActionType stringToRequestActionType(std::string _requestActionType);
                /**
                * returns a string with the action code and the option parameters
                */
                EXPORT std::string getRequestInfo();
                /**
                * returns true if the request may be executed (if all options / query values are there for processing the request)
                */
                EXPORT virtual bool isValid();
                /**
                * returns the errors if there are one
                */
                EXPORT std::string getErrors();
                /**
                *  craete an requestAction object from an url
                */
                EXPORT static std::shared_ptr<RequestAction> createFromUrl(std::string _url);
                /**
                *  craeted an requestAction object from an path
                */
                EXPORT static std::shared_ptr<RequestAction> createFromPath(std::string _path, std::string _queryString);
                /**
                * returns a room UDN for a room UDN or a room name
                */
                EXPORT std::string getRoomUDNFromId(std::string _id);
                /**
                * returns a zone UDN for a room UDN or a room name or a zoneUDN
                */
                EXPORT std::string getZoneUDNFromId(std::string _id);
     
            protected:                
                /**
                * parses the query options to a map
                */
                virtual void parseQueryOptions();
                /**
                * returns a shared pointer to a vertiual media renderer (zone)
                * all actions will be performed on that rendeerer (even volume for each rooms)
                */
                virtual std::shared_ptr<Raumkernel::Devices::MediaRenderer_RaumfeldVirtual> getVirtualMediaRenderer(std::string _id);
                /**
                * 
                */
                virtual std::string getOptionValue(std::string _key, std::string _default = "");
                /**
                * 
                */
                virtual std::vector<std::string> getOptionValueMultiple(std::string _key, std::string _delimiter = ",");
                /**
                * the execute action itself
                */
                EXPORT virtual bool executeAction();
                /**
                * returns if the scope is a zone scope
                */
                EXPORT virtual bool isZoneScope(const std::string &_scope);
           
                virtual void logError(const std::string &_log, const std::string &_location) override;
                virtual void logCritical(const std::string &_log, const std::string &_location) override;

                /**
                * the whole url of the request
                */
                std::string url;
                /**
                * the query of the request
                */
                std::string query;
                /**
                * the action we have to perform
                */
                RequestActionType action;
                /**
                * the receiver of the request (may be a zone or a room or.....)
                */
                RequestReceiver receiver;
                /**
                * if set the executor will wait with the response until the requestAction is finished
                */
                bool sync;
                /**
                * query values as request options
                */
                std::unordered_map<std::string, std::string> requestOptions;   
                /**
                * contains some error if the request may not be executed. This error may be written back to the user
                */
                std::string error;
                /**
                * wait time after the execution of a request (is used to be sure the kernel as got the new changed values via the update sbscription)
                * this is only necessary on some special requests
                */
                std::uint16_t waitTimeAfterExecution;
                /**
                * timout for processing the whole request
                */
                std::uint16_t timeout;
                /**
                * wait time to check the kernel for updates
                */
                std::uint16_t waitTimeForRequestActionKernelResponse;
        };


        /**
        * The RequestActionReturnable class is a special requetAction class
        * This one can return a response string to the client which requested the action
        */
        class RequestActionReturnable : public RequestAction
        {
            public:
                EXPORT RequestActionReturnable(std::string _url);
                EXPORT RequestActionReturnable(std::string _path, std::string _query);                
                EXPORT virtual ~RequestActionReturnable();              
                
                EXPORT virtual bool isStackable();
                EXPORT virtual bool isAsyncExecutionAllowed();

            protected:                     
       };

    }
}


#endif