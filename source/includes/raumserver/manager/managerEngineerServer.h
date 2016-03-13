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
#ifndef RAUMSERVER_MANAGERENGINEERSERVER_H
#define RAUMSERVER_MANAGERENGINEERSERVER_H

#include <raumserver/raumserverBase.h>
#include <raumserver/manager/requestActionManager.h>

namespace Raumserver
{
    namespace Manager
    {
        /*
        This class holds pointers to all the available managers loaded in the server object
        */
        class ManagerEngineerServer : public RaumserverBase
        {
            public:
                EXPORT ManagerEngineerServer();
                EXPORT virtual ~ManagerEngineerServer();
                void createManagers();

                EXPORT std::shared_ptr<Manager::RequestActionManager> getRequestActionManager();              

            protected:
                std::shared_ptr<Manager::RequestActionManager> requestActionManager;
               
        };
    }
}


#endif