#include <sstream>
#include <raumserver/json//json.h>
#include "raumserver/webserver/mongoose/cpp/JsonResponse.h"

using namespace std;

namespace Mongoose
{
    JsonResponse::JsonResponse()
        : humanReadable(false)
    {
    }

    string JsonResponse::getBody()
    {
        if (humanReadable) {
            Json::StyledWriter writer;
            return writer.write(*this);
        } else {
            Json::FastWriter writer;
            return writer.write(*this);
        }
    }

    void JsonResponse::setHuman(bool human)
    {
        humanReadable = human;
    }
}
