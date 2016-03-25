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
#ifndef RAUMKERNEL_RAUMSERVER_H
#define RAUMKERNEL_RAUMSERVER_H

#include <raumkernel/raumkernel.h>
#include <raumserver/webserver/webserver.h>
#include <raumserver/manager/managerEngineerServer.h>


namespace Raumserver
{

    const std::string SETTINGS_RAUMSERVER_PORT_DEFAULT = "8080";
    const std::string SETTINGS_RAUMSERVER_PORT = "/Raumserver/Port";

    class Raumserver : public Raumkernel::RaumkernelBase
    {
        public:
            EXPORT Raumserver();
            EXPORT virtual ~Raumserver();
            EXPORT virtual void init(Raumkernel::Log::LogType _defaultLogLevel = Raumkernel::Log::LogType::LOGTYPE_ERROR);
            /**
            * returns a shared pointer to the raumkernel object
            */
            EXPORT std::shared_ptr<Raumkernel::Raumkernel> getRaumkernelObject();
            /**
            * returns if the raumfeldSystem is online
            */
            virtual bool isRaumserverOnline();
            /**
            * returns the version info object/structure for the raumkernel
            */
            EXPORT virtual Raumkernel::Tools::VersionInfo getVersionInfo();
            /**
            * this signal will be fired if a the raumkernel is ready (
            * (in fact that means that the media server is online!)
            */
            sigs::signal<void(bool _server, bool _kernel)> sigRaumserverOnline;
            /**
            * this signal will be fired if a the raumkernel is ready (
            * (in fact that will be if the media server goes offline!)
            */
            sigs::signal<void(bool _server, bool _kernel)> sigRaumserverOffline;

        protected:

            void onRaumfeldSystemOnline();
            void onRaumfeldSystemOffline();

            std::shared_ptr<Raumkernel::Raumkernel> raumkernel;
            std::shared_ptr<Raumkernel::Manager::ManagerEngineer> managerEngineerKernel;
            std::shared_ptr<Manager::ManagerEngineerServer> managerEngineerServer;
            std::shared_ptr<Server::Webserver> webserver;

            Raumkernel::Tools::VersionInfo versionInfo;

            bool isOnline;

            sigs::connections connections;
    };

}


#endif