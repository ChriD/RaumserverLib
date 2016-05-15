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
#ifndef RAUMSERVER_REQUESTACTIONMANAGER_H
#define RAUMSERVER_REQUESTACTIONMANAGER_H

#include <queue>
#include <thread>
#include <raumkernel/versionInfo.h>
#include <raumserver/manager/managerBaseServer.h>
#include <raumserver/request/requestActions.h>


namespace Raumserver
{
    namespace Manager
    {        
        class RequestActionManager : public ManagerBaseServer
        {
            public:
                EXPORT RequestActionManager();
                EXPORT virtual ~RequestActionManager(); 
                /**
                * starting the thread for processing requests
                */
                EXPORT virtual void init();
                /**
                * add a requestAction to the queue which will be processed by the thread method
                */
                EXPORT virtual void addRequestAction(std::shared_ptr<Request::RequestAction> _requestAction);
                /**
                * The thread method which will process the queue
                */
                EXPORT virtual void requestProcessingWorkerThread();
                /**
                * Set Server Version Info for request responses
                */
                EXPORT virtual void setServerVersion(const VersionInfo::VersionInfo &_versionInfo);
                /**
                * Set Kernel Version Info for request responses
                */
                EXPORT virtual void setKernelVersion(const VersionInfo::VersionInfo &_versionInfo);
                /**
                * Get Server Version Info for request responses
                */
                EXPORT virtual VersionInfo::VersionInfo getServerVersion();
                /**
                * Get Kernel Version Info for request responses
                */
                EXPORT virtual VersionInfo::VersionInfo getKernelVersion();

            protected:          

                /**
                * this method runs as thread and checks id the stack is filled with some requests
                * if so it will perform the requests in a FIFO order
                */
                void doRequests();
                std::thread doRequestsThreadObject;
                std::atomic_bool stopThreads;

                // a mutex that will secure our request action list 
                std::mutex mutexRequestActionQueue;              

                // a list which contains all request Actions whch are not already processed                    
                std::queue<std::shared_ptr<Request::RequestAction>> requestActionQueue;

                // Version info only for returning om request responses)
                VersionInfo::VersionInfo versionInfoKernel;
                VersionInfo::VersionInfo versionInfoServer;
                
                
        };
    }
}


#endif