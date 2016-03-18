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

#include <raumserver/raumserverBaseMgr.h>
#include <raumkernel/manager/managerEngineer.h>
#include <raumkernel/manager/zoneManager.h>
#include <raumkernel/manager/deviceManager.h>

namespace Raumserver
{
    namespace Request
    {
        enum class RequestActionType { RAA_UNDEFINED, RAA_PLAY, RAA_PAUSE, RAA_STOP, RAA_NEXT, RAA_PREV, RAA_VOLUMEUP, RAA_VOLUMEDOWN, RAA_SETVOLUME };
        enum class RequestReceiver { RR_ROOM, RR_ZONE, RR_JSON };
     
        class RequestAction : public RaumserverBaseMgr
        {
            public:
                EXPORT RequestAction();
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
                virtual bool isValid();
     
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
                * returns true if the request may be executed (if all options / query values are there for processing the request)
                */
                virtual std::string getOptionValue(std::string _key, std::string _default = "");
                /**
                * the execute action itself
                */
                EXPORT virtual bool executeAction();
           
                virtual void logError(std::string _log, std::string _location) override;
                virtual void logCritical(std::string _log, std::string _location) override;

                /**
                * the whole url of the request
                */
                std::string url;
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
        };


        /**
        * The RequestActionReturnable class is a special requetAction class
        * This one can return a response string to the client which requested the action
        */
        class RequestActionReturnable : public RequestAction
        {
            public:
                EXPORT RequestActionReturnable();
                EXPORT virtual ~RequestActionReturnable();              
                
                EXPORT virtual bool isStackable();
                EXPORT virtual bool isAsyncExecutionAllowed();

            protected:                     
       };

    }
}


#endif