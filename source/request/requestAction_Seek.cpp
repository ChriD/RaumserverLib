
#include <raumserver/request/requestAction_Seek.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_Seek::RequestAction_Seek(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_SEEK;
        }


        RequestAction_Seek::RequestAction_Seek(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_SEEK;
        }


        RequestAction_Seek::~RequestAction_Seek()
        {
        }


        bool RequestAction_Seek::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:            
            // raumserver/controller/seek?id=Schlafzimmer&value=3000&seektype=abs
            // raumserver/controller/seek?id=Schlafzimmer&value=-3000&seektype=rel
            // raumserver/controller/seek?id=Schlafzimmer&value=4&seektype=track
            // raumserver/controller/seek?id=Schlafzimmer&value=150000
            // raumserver/controller/seek?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88&value=150000

            auto id = getOptionValue("id");
            auto valueString = getOptionValue("value");

            if (id.empty())
            {
                logError("'id' option is needed to execute 'seek' command!", CURRENT_FUNCTION);
                isValid = false;
            }
            if (valueString.empty())
            {
                logError("'value' option is needed to execute 'seek' command!", CURRENT_FUNCTION);
                isValid = false;
            }

            return isValid;
        }


        bool RequestAction_Seek::executeAction()
        {
            auto id = getOptionValue("id");
            auto valueString = getOptionValue("value");
            auto seekTypeString = getOptionValue("seektype");

            // if we got an id we try to stop the playing for the id (which may be a roomUDN, a zoneUDM or a roomName)
            if (!id.empty())
            {
                auto mediaRenderer = getVirtualMediaRenderer(id);
                if (!mediaRenderer)
                {
                    logError("Room or Zone with ID: " + id + " not found!", CURRENT_FUNCTION);
                    return false;
                }

                // convert the seek string to a enum
                auto seekType = Raumkernel::Devices::MediaRenderer_Seek::MRSEEK_ABS_TIME;
                if (!seekTypeString.empty())
                {
                    if (Raumkernel::Tools::StringUtil::tolower(seekTypeString) == "rel" || Raumkernel::Tools::StringUtil::tolower(seekTypeString) == "relative") seekType = Raumkernel::Devices::MediaRenderer_Seek::MRSEEK_REL_TIME;
                    if (Raumkernel::Tools::StringUtil::tolower(seekTypeString) == "track") seekType = Raumkernel::Devices::MediaRenderer_Seek::MRSEEK_TRACK_NR;
                }

                // convert the value to an integer
                auto msOrTrack = Raumkernel::Tools::CommonUtil::toInt32(valueString);

                // load the current media info from the renderer (we may get it from the subscripted info but to be save we get it directly)
                auto mediaInfo = mediaRenderer->getMediaInfo(true);

                if (seekType != Raumkernel::Devices::MediaRenderer_Seek::MRSEEK_REL_TIME)
                {
                    if (msOrTrack < 0)
                        msOrTrack = 0;
                }

                if (seekType == Raumkernel::Devices::MediaRenderer_Seek::MRSEEK_TRACK_NR)
                {
                    if ((std::uint32_t)msOrTrack > mediaInfo.nrTracks)
                        msOrTrack = mediaInfo.nrTracks - 1;
                }

                mediaRenderer->seek(seekType, msOrTrack, sync);
            }            

            return true;
        }
    }
}
