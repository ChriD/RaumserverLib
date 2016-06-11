//
// The MIT License (MIT)
//
// Copyright (c) 2016 by ChriD
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
#ifndef RAUMSERVER_REQUESTACTIONRETURNABLE_H
#define RAUMSERVER_REQUESTACTIONRETURNABLE_H

#include <raumserver/request/requestAction.h>
#include <raumserver/json/rapidjson/rapidjson.h>
#include <raumserver/json/rapidjson/writer.h>
#include <raumserver/json/rapidjson/stringbuffer.h>

namespace Raumserver
{
    namespace Request
    {
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
                EXPORT std::string getResponseData();
                EXPORT std::map<std::string, std::string> getResponseHeader();

            protected:
                void setResponseData(const std::string &_data);
                void addResponseHeader(const std::string &_key, const std::string &_value);

                std::string responseData;
                std::map<std::string, std::string> responseHeader;
        };
    }
}


#endif