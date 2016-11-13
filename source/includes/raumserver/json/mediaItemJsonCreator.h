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
#ifndef RAUMSERVER_MEDIAITEMJSONCREATOR_H
#define RAUMSERVER_MEDIAITEMJSONCREATOR_H

#include <raumkernel/media/item/mediaItems.h>
#include <raumserver/json/rapidjson/rapidjson.h>
#include <raumserver/json/rapidjson/writer.h>
#include <raumserver/json/rapidjson/stringbuffer.h>

namespace Raumserver
{
    class MediaItemJsonCreator
    {
        public:

            static void addJson(std::shared_ptr<Raumkernel::Media::Item::MediaItem> _mediaItem, rapidjson::Writer<rapidjson::StringBuffer> &_jsonWriter)
            {
                addJsonForMediaItem(_mediaItem, _jsonWriter);                
                if (std::dynamic_pointer_cast<Raumkernel::Media::Item::MediaItem_Container>(_mediaItem)) addJsonForMediaItem_Container(std::dynamic_pointer_cast<Raumkernel::Media::Item::MediaItem_Container>(_mediaItem), _jsonWriter);
                if (std::dynamic_pointer_cast<Raumkernel::Media::Item::MediaItem_Artist>(_mediaItem)) addJsonForMediaItem_Artist(std::dynamic_pointer_cast<Raumkernel::Media::Item::MediaItem_Artist>(_mediaItem), _jsonWriter);
                if (std::dynamic_pointer_cast<Raumkernel::Media::Item::MediaItem_Album>(_mediaItem)) addJsonForMediaItem_Album(std::dynamic_pointer_cast<Raumkernel::Media::Item::MediaItem_Album>(_mediaItem), _jsonWriter);
                if (std::dynamic_pointer_cast<Raumkernel::Media::Item::MediaItem_Track>(_mediaItem)) addJsonForMediaItem_Track(std::dynamic_pointer_cast<Raumkernel::Media::Item::MediaItem_Track>(_mediaItem), _jsonWriter);                
                if (std::dynamic_pointer_cast<Raumkernel::Media::Item::MediaItem_Radio_RadioTime>(_mediaItem)) addJsonForMediaItem_Radio_RadioTime(std::dynamic_pointer_cast<Raumkernel::Media::Item::MediaItem_Radio_RadioTime>(_mediaItem), _jsonWriter);
                if (std::dynamic_pointer_cast<Raumkernel::Media::Item::MediaItem_Radio_Rhapsody>(_mediaItem)) addJsonForMediaItem_Radio_Rhapsody(std::dynamic_pointer_cast<Raumkernel::Media::Item::MediaItem_Radio_Rhapsody>(_mediaItem), _jsonWriter);
                // TODO: @@@
                // we have to add more
            }

            static void addJsonForMediaItem(std::shared_ptr<Raumkernel::Media::Item::MediaItem> _mediaItem, rapidjson::Writer<rapidjson::StringBuffer> &_jsonWriter)
            {
                _jsonWriter.Key("id"); _jsonWriter.String(_mediaItem->id.c_str());
                _jsonWriter.Key("parentId"); _jsonWriter.String(_mediaItem->parentId.c_str());                
                _jsonWriter.Key("type"); _jsonWriter.String(Raumkernel::Media::Item::MediaItem::mediaItemTypeToString(_mediaItem->type).c_str());
            }

            static void addJsonForMediaItem_Container(std::shared_ptr<Raumkernel::Media::Item::MediaItem_Container> _mediaItem, rapidjson::Writer<rapidjson::StringBuffer> &_jsonWriter)
            {                
                _jsonWriter.Key("title"); _jsonWriter.String(_mediaItem->title.c_str());
                _jsonWriter.Key("description"); _jsonWriter.String(_mediaItem->description.c_str());
            }


            static void addJsonForMediaItem_Artist(std::shared_ptr<Raumkernel::Media::Item::MediaItem_Artist> _mediaItem, rapidjson::Writer<rapidjson::StringBuffer> &_jsonWriter)
            {
                _jsonWriter.Key("artist"); _jsonWriter.String(_mediaItem->artist.c_str());
                _jsonWriter.Key("artistArtUri"); _jsonWriter.String(_mediaItem->artistArtUri.c_str());                
            }


            static void addJsonForMediaItem_Album(std::shared_ptr<Raumkernel::Media::Item::MediaItem_Album> _mediaItem, rapidjson::Writer<rapidjson::StringBuffer> &_jsonWriter)
            {
                //addJsonForMediaItem_Artist(std::dynamic_pointer_cast<Raumkernel::Media::Item::MediaItem_Artist>(_mediaItem), _jsonWriter);
                _jsonWriter.Key("album"); _jsonWriter.String(_mediaItem->album.c_str());
                _jsonWriter.Key("albumArtUri"); _jsonWriter.String(_mediaItem->albumArtUri.c_str());
                _jsonWriter.Key("albumDate"); _jsonWriter.String(_mediaItem->albumDate.c_str());
                _jsonWriter.Key("albumTotalPlaytime"); _jsonWriter.String(_mediaItem->albumTotalPlaytime.c_str());
                _jsonWriter.Key("albumTrackCount"); _jsonWriter.Int(_mediaItem->albumTrackCount);
            }


            static void addJsonForMediaItem_Track(std::shared_ptr<Raumkernel::Media::Item::MediaItem_Track> _mediaItem, rapidjson::Writer<rapidjson::StringBuffer> &_jsonWriter)
            {
                //addJsonForMediaItem_Album(std::dynamic_pointer_cast<Raumkernel::Media::Item::MediaItem_Album>(_mediaItem), _jsonWriter);
                _jsonWriter.Key("title"); _jsonWriter.String(_mediaItem->title.c_str());                
            }


            static void addJsonForMediaItem_Radio(std::shared_ptr<Raumkernel::Media::Item::MediaItem_Radio> _mediaItem, rapidjson::Writer<rapidjson::StringBuffer> &_jsonWriter)
            {       
                _jsonWriter.Key("title"); _jsonWriter.String(_mediaItem->title.c_str());
                _jsonWriter.Key("description"); _jsonWriter.String(_mediaItem->description.c_str());
                _jsonWriter.Key("albumArtUri"); _jsonWriter.String(_mediaItem->albumArtUri.c_str());
                _jsonWriter.Key("region"); _jsonWriter.String(_mediaItem->region.c_str());
                _jsonWriter.Key("signalStrength"); _jsonWriter.Int(_mediaItem->signalStrength);
                _jsonWriter.Key("durability"); _jsonWriter.Int(_mediaItem->durability);
                _jsonWriter.Key("bitrate"); _jsonWriter.Int(_mediaItem->bitrate);            
            }


            static void addJsonForMediaItem_Radio_RadioTime(std::shared_ptr<Raumkernel::Media::Item::MediaItem_Radio_RadioTime> _mediaItem, rapidjson::Writer<rapidjson::StringBuffer> &_jsonWriter)
            {                
                //addJsonForMediaItem_Radio(std::dynamic_pointer_cast<Raumkernel::Media::Item::MediaItem_Radio>(_mediaItem), _jsonWriter);                
            }


            static void addJsonForMediaItem_Radio_Rhapsody(std::shared_ptr<Raumkernel::Media::Item::MediaItem_Radio_Rhapsody> _mediaItem, rapidjson::Writer<rapidjson::StringBuffer> &_jsonWriter)
            {
                //addJsonForMediaItem_Radio(std::dynamic_pointer_cast<Raumkernel::Media::Item::MediaItem_Radio>(_mediaItem), _jsonWriter);
            }

            // TODO: @@@
            // we have to add more
     
        protected:
    };
}

#endif