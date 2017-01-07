
#include <raumserver/request/requestAction_LoadShuffle.h>

namespace Raumserver
{
    namespace Request
    {
        RequestAction_LoadShuffle::RequestAction_LoadShuffle(std::string _url) : RequestAction(_url)
        {
            action = RequestActionType::RAA_LOADSHUFFLE;
            listRetrieved = false;
            shuffleContainerId = "";
        }


        RequestAction_LoadShuffle::RequestAction_LoadShuffle(std::string _path, std::string _query) : RequestAction(_path, _query)
        {
            action = RequestActionType::RAA_LOADSHUFFLE;
            listRetrieved = false;
            shuffleContainerId = "";
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


        void RequestAction_LoadShuffle::onMediaListDataChanged(std::string _listId)
        {
            if (!_listId.empty() && shuffleContainerId == _listId)
                listRetrieved = true;
        }


        bool RequestAction_LoadShuffle::executeAction()
        {
            auto id = getOptionValue("id");
            auto source = Raumkernel::Tools::StringUtil::tolower(getOptionValue("source"));
            auto selection = getOptionValue("selection");

            getManagerEngineer()->getDeviceManager()->lock();
            getManagerEngineer()->getZoneManager()->lock();

            try
            {
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

                    //connections.connect(managerEngineer->getMediaListManager()->sigMediaListDataChanged, this, &RequestAction_LoadShuffle::onMediaListDataChanged);

                    //auto lastUpdateId = mediaRenderer->getLastRendererStateUpdateId();

                    mediaRenderer->loadShuffle(source, selection);

                    //auto state = mediaRenderer->getTransportInfo().currentTransportState;
                    //auto status = mediaRenderer->getTransportInfo().currentTransportStatus;

                    /*
                    // I dont know when the fucking shuffle is loaded and ready.....Damn!
                    // TODO: Wait till transport state of renderer is != TRANSITIONING
                    std::this_thread::sleep_for(std::chrono::milliseconds(150));
                    while (true)
                    {
                        //auto state = mediaRenderer->getTransportInfo().currentTransportState;
                        //auto status = mediaRenderer->getTransportInfo().currentTransportStatus;

                        auto curLastUpdateId = mediaRenderer->getLastRendererStateUpdateId();
                        if (lastUpdateId != curLastUpdateId)
                        {
                            if (mediaRenderer->state().transportState != Raumkernel::Devices::MediaRenderer_TransportState::MRTS_TRANSITIONING);
                                break;
                            lastUpdateId = curLastUpdateId;                        
                        }
                        std::this_thread::sleep_for(std::chrono::milliseconds(250));
                    }
                    */

                    // TODO: wait till list is loaded in the list manager?!
                
                    // wait till the list is loaded by the media lits manager. if the list was loaded from cache
                    // we do not have to wait for it (would lead to an endless loop)
                    // while (!listRetrieved)
                    //{
                    //    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    //}                
                }
            }
            catch (...)
            {
                logError("Unknown Exception!", CURRENT_POSITION);
            }

            getManagerEngineer()->getDeviceManager()->unlock();
            getManagerEngineer()->getZoneManager()->unlock();

            return true;
        }
    }
}
