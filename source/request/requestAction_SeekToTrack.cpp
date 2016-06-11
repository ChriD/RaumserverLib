
#include <raumserver/request/requestAction_SeekToTrack.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_SeekToTrack::RequestAction_SeekToTrack(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_SEEKTOTRACK;
        }


        RequestAction_SeekToTrack::RequestAction_SeekToTrack(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_SEEKTOTRACK;
        }


        RequestAction_SeekToTrack::~RequestAction_SeekToTrack()
        {
        }


        bool RequestAction_SeekToTrack::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:            
            // raumserver/controller/seekToTrack?id=Schlafzimmer&trackIndex=3
            // raumserver/controller/seekToTrack?id=Schlafzimmer&trackNumber=4
            // raumserver/controller/seekToTrack?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88&trackIndex=0

            auto id = getOptionValue("id");
            auto trackIndexString = getOptionValue("trackIndex");
            auto trackNumberString = getOptionValue("trackNumber");

            if (id.empty())
            {
                logError("'id' option is needed to execute 'seekToTrack' command!", CURRENT_FUNCTION);
                isValid = false;
            }
            if (trackIndexString.empty() && trackNumberString.empty())
            {
                logError("'trackIndex' or 'trackNumber' option is needed to execute 'seekToTrack' command!", CURRENT_FUNCTION);
                isValid = false;
            }

            return isValid;
        }


        bool RequestAction_SeekToTrack::executeAction()
        {
            auto id = getOptionValue("id");
            auto trackIndexString = getOptionValue("trackIndex");
            auto trackNumberString = getOptionValue("trackNumber");

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
                auto seekType = Raumkernel::Devices::MediaRenderer_Seek::MRSEEK_TRACK_NR;           
                
                std::int32_t trackIndex = 0;
                if (!trackNumberString.empty())
                    trackIndex = Raumkernel::Tools::CommonUtil::toInt32(trackNumberString) - 1;
                else
                    trackIndex = Raumkernel::Tools::CommonUtil::toInt32(trackIndexString);              

                // load the current media info from the renderer (we may get it from the subscripted info but to be save we get it directly)
                auto mediaInfo = mediaRenderer->getMediaInfo(true);

                if (trackIndex < 0)
                    trackIndex = 0;

                if ((std::uint32_t)trackIndex > mediaInfo.nrTracks)
                    trackIndex = mediaInfo.nrTracks - 1;              

                // only seek to track if there is a ist to seek!
                if (mediaInfo.nrTracks > 1)
                    mediaRenderer->seek(seekType, trackIndex + 1, sync);
            }

            return true;
        }
    }
}
