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

namespace Raumserver
{
    namespace Request
    {
        enum class RequestActionType { RAA_PLAY, RAA_PAUSE, RAA_STOP, RAA_NEXT, RAA_PREV, RAA_VOLUMEUP, RAA_VOLUMEDOWN, RAA_SETVOLUME};
     
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
                EXPORT virtual void execute();
     
            protected:

                /**
                * the whole url of the request
                */
                std::string url;
                /**
                * the action we have to perform
                */
                RequestActionType action;
                /**
                * if set the executor will wait with the response until the requestAction is finished
                */
                bool sync;
                /**
                * TODO: query values as options?!
                */
                //map<std::string, std::string>                
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