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
#ifndef RAUMKERNEL_WEBSERVER_H
#define RAUMKERNEL_WEBSERVER_H

#include <thread>
#include <chrono>

#include <raumkernel/raumkernel.h>
#include <raumserver/request/requestAction.h>
#include <raumserver/raumserverBase.h>
#include <raumserver/manager/managerEngineerServer.h>
#include <raumserver/webserver/civetweb/civetServer.h>

#define DOCUMENT_ROOT "docroot"

#define fopen_recursive fopen

namespace Raumserver
{
    namespace Server
    {

        class RequestHandlerBase : public CivetHandler
        {
            public:
                EXPORT void setManagerEngineerServer(std::shared_ptr<Manager::ManagerEngineerServer> _managerEngineer);
                EXPORT void setManagerEngineerKernel(std::shared_ptr<Raumkernel::Manager::ManagerEngineer> _managerEngineer);
                EXPORT void setLogObject(std::shared_ptr<Raumkernel::Log::Log> _logObject);               
                EXPORT std::shared_ptr<Manager::ManagerEngineerServer> getManagerEngineerServer();
                EXPORT std::shared_ptr<Raumkernel::Manager::ManagerEngineer> getManagerEngineerKernel();
                EXPORT std::shared_ptr<Raumkernel::Log::Log> getLogObject();
            protected:
                virtual std::string buildCorsHeader(std::map<std::string, std::string>* _headerVars = nullptr);
                virtual void sendResponse(struct mg_connection *_conn, std::string _string, bool _error = false);
                virtual void sendDataResponse(struct mg_connection *_conn, std::string _string, std::map<std::string,std::string> _headerVars = std::map<std::string, std::string>(), bool _error = false);
                std::shared_ptr<Manager::ManagerEngineerServer> managerEngineerServer;
                std::shared_ptr<Raumkernel::Manager::ManagerEngineer> managerEngineerKernel;
                std::shared_ptr<Raumkernel::Log::Log> logObject;                
        };



        class RequestHandlerController : public RequestHandlerBase
        {
            public:
                bool handleGet(CivetServer *_server, struct mg_connection *_conn) override;  
                bool handleOptions(CivetServer *_server, struct mg_connection *_conn) override;
        };


        class RequestHandlerData : public RequestHandlerController
        {
            public:
                bool handleGet(CivetServer *_server, struct mg_connection *_conn) override;   
                bool handleOptions(CivetServer *_server, struct mg_connection *_conn) override;
        };


        class Webserver : public RaumserverBaseMgr
        {
            public:
                EXPORT Webserver();
                EXPORT virtual ~Webserver();    
                EXPORT virtual void start(std::uint32_t _port);
                EXPORT virtual void stop();
                EXPORT virtual void setDocumentRoot(std::string _docroot);

            protected:                  
                std::shared_ptr<CivetServer> serverObject;             
                std::shared_ptr<RequestHandlerController> serverRequestHandlerController;
                std::shared_ptr<RequestHandlerData> serverRequestHandlerData;           

                bool isStarted;
                std::string docroot;
        };
  

    }
}


#endif