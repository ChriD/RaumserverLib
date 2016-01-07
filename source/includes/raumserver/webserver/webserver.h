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
#include <raumserver/webserver/mongoose/cpp/Server.h>
#include <raumserver/webserver/mongoose/cpp/WebController.h>

namespace Raumserver
{
    namespace Server
    {

        class Webserver : public Raumkernel::RaumkernelBase
        {
            public:
                EXPORT Webserver();
                EXPORT virtual ~Webserver();    
                EXPORT virtual void start(std::uint32_t _port);
                EXPORT virtual void stop();

            protected:   
                std::shared_ptr<Mongoose::Server> server;
                std::shared_ptr<Mongoose::WebController> requestController;
                bool isStarted;
        };


        class RequestController : public Mongoose::WebController
        {
            public:
                void handleRequest(Mongoose::Request &_request, Mongoose::StreamResponse &_response);
                void setup();                         
        };

    }
}


#endif