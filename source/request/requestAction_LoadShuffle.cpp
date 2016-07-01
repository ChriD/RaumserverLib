
#include <raumserver/request/requestAction_LoadShuffle.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_LoadShuffle::RequestAction_LoadShuffle(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_LOADSHUFFLE;
        }


        RequestAction_LoadShuffle::RequestAction_LoadShuffle(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_LOADSHUFFLE;
        }


        RequestAction_LoadShuffle::~RequestAction_LoadShuffle()
        {
        }


        bool RequestAction_LoadShuffle::isValid()
        {
            bool isValid = RequestAction::isValid();

            // examples for valid requests:            
            // raumserver/controller/loadShuffle?id=Schlafzimmer&source=recentArtists
            // raumserver/controller/loadShuffle?id=uuid:3f68f253-df2a-4474-8640-fd45dd9ebf88&source=topArtists

            auto id = getOptionValue("id");
            auto source = getOptionValue("source");

            if (id.empty())
            {
                logError("'id' option is needed to execute 'loadShuffle' command!", CURRENT_FUNCTION);
                isValid = false;
            }
            if (source.empty())
            {
                logError("'source' option is needed to execute 'loadShuffle' command!", CURRENT_FUNCTION);
                isValid = false;
            }

            return isValid;
        }


        bool RequestAction_LoadShuffle::executeAction()
        {
            auto id = getOptionValue("id");
            auto source = getOptionValue("source");
            auto selection = getOptionValue("selection");

            // if we got an id we try to stop the playing for the id (which may be a roomUDN, a zoneUDM or a roomName)
            if (!id.empty())
            {
                auto mediaRenderer = getVirtualMediaRenderer(id);
                if (!mediaRenderer)
                {
                    logError("Room or Zone with ID: " + id + " not found!", CURRENT_FUNCTION);
                    return false;
                }

                // type may be a direct container where to shuffle or a synonym
                if (source == "recentartists")
                    source = "0/Playlists/Shuffles/RecentArtists";
                if (source == "topartists")
                    source = "0/Playlists/Shuffles/TopArtists";
                if (source == "all")
                    source = "0/Playlists/Shuffles/All";
                // on following types we can add selections
                if (source == "genre")
                    source = "0/Playlists/Shuffles/Genre";
                if (source == "genre")
                    source = "0/Playlists/Shuffles/Artists";

                /*
                object.container.playlistContainer.shuffle 0/Playlists/Shuffles/RecentArtists
                object.container.playlistContainer.shuffle 0/Playlists/Shuffles/TopArtists
                object.container.playlistContainer.shuffle 0/Playlists/Shuffles/All
                object.container.playlistContainer.shuffle.search 0/Playlists/Shuffles/Genre
                object.container.playlistContainer.shuffle.search 0/Playlists/Shuffles/Artists
                */

                mediaRenderer->loadShuffle(source, selection);
            }

            return true;
        }
    }
}
