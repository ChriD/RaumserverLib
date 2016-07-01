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
#ifndef RAUMSERVER_REQUESTACTIONS_H
#define RAUMSERVER_REQUESTACTIONS_H

#include <raumserver/request/requestAction_Pause.h>
#include <raumserver/request/requestAction_Play.h>
#include <raumserver/request/requestAction_Stop.h>
#include <raumserver/request/requestAction_Next.h>
#include <raumserver/request/requestAction_Prev.h>
#include <raumserver/request/requestAction_CreateZone.h>
#include <raumserver/request/requestAction_DropFromZone.h>
#include <raumserver/request/requestAction_AddToZone.h>
#include <raumserver/request/requestAction_Mute.h>
#include <raumserver/request/requestAction_Unmute.h>
#include <raumserver/request/requestAction_SetPlayMode.h>
#include <raumserver/request/requestAction_VolumeUp.h>
#include <raumserver/request/requestAction_VolumeDown.h>
#include <raumserver/request/requestAction_SetVolume.h>
#include <raumserver/request/requestAction_LoadUri.h>
#include <raumserver/request/requestAction_LoadPlaylist.h>
#include <raumserver/request/requestAction_LoadContainer.h>
#include <raumserver/request/requestAction_Seek.h>
#include <raumserver/request/requestAction_SeekToTrack.h>
#include <raumserver/request/requestAction_ToggleMute.h>
#include <raumserver/request/requestAction_FadeToVolume.h>
#include <raumserver/request/requestAction_SleepTimer.h>
#include <raumserver/request/requestAction_LoadShuffle.h>

#include <raumserver/request/requestActionReturnable_GetVersion.h>

#include <raumserver/request/requestActionReturnableLP_GetZoneConfig.h>
#include <raumserver/request/requestActionReturnableLP_GetMediaList.h>
#include <raumserver/request/requestActionReturnableLP_GetZoneMediaList.h>
#include <raumserver/request/requestActionReturnableLP_GetRendererState.h>
#include <raumserver/request/requestActionReturnableLP_GetRendererTransportState.h>

#endif