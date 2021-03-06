/*
 * Copyright (c) 2019, Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.

 *
 */

//!
//! \file     RenderType.h
//! \brief    define basic structure and data.
//!

#ifndef _RENDERTYPE_H_
#define _RENDERTYPE_H_

#include <stdint.h>
#include <string>
#include <vector>
#include "360SCVPAPI.h"

typedef bool bool_t;
typedef char char_t;
typedef char utf8_t;

typedef float float32_t;
typedef double float64_t;

#ifndef NULL
#define NULL 0
#endif

#ifndef RENDER_PI
#define RENDER_PI 3.1415926f
#endif

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/pixfmt.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

namespace PixelFormat
{
enum Enum
{
    INVALID,
    PIX_FMT_RGB24,
    PIX_FMT_YUV420P,
    AV_PIX_FMT_NV12,
    AV_PIX_FMT_NV12_DMA_BUFFER
};
};

namespace MediaSourceType
{
enum Enum
{
    SOURCE_NONE,
    SOURCE_VOD = 1,
    SOURCE_LIVE
};
};
enum
{
    DASH_SOURCE = 0,
    FFMPEG_SOURCE,
    WEBRTC_SOURCE
};
enum
{
    SW_DECODER = 0,
    VAAPI_DECODER
};
enum RenderContextType
{
    DEFAULT_CONTEXT,
    GLFW_CONTEXT = 0,
    EGL_CONTEXT
};
enum
{
    READY = 0,
    PLAY,
    PAUSE
};
enum TileType
{
    HIGHRESOTYPE = 0,
    LOWRESOTYPE,
};
struct RenderConfig
{
    //from config
    uint32_t windowWidth;
    uint32_t windowHeight;
    const char *url;
    uint32_t sourceType;
    uint32_t decoderType;
    uint32_t contextType;
    uint32_t useDMABuffer;
    uint32_t viewportHFOV;
    uint32_t viewportVFOV;
    uint32_t viewportWidth;
    uint32_t viewportHeight;
    const char *cachePath;
    //from media source
    int32_t projFormat;
    uint32_t renderInterval;
};

enum ThreadStatus{
    STATUS_CREATED=0,
    STATUS_RUNNING,
    STATUS_STOPPED,
    STATUS_UNKNOWN,
};
#ifdef LOW_LATENCY_USAGE
enum ProjectType{
    PT_UNKNOWN     = -1,
    PT_ERP         =  0,
    PT_CUBEMAP     =  1,
    PT_RESERVED,
};
#endif
struct SphereRegion
{
    uint8_t viewIdc; //corresponding to view_idc[i] when view_idc_presence_flag is equal to 1
    int32_t centreAzimuth;
    int32_t centreElevation;
    int32_t centreTilt;
    uint32_t azimuthRange;
    uint32_t elevationRange;
    bool interpolate; // can only be 0 here
};

struct SourceWH
{
    uint32_t *width;
    uint32_t *height;
};

struct FrameInfo
{
    uint8_t **mBuffer;
    struct RegionInfo *mRegionInfo;
};

struct MediaSourceInfo
{
    uint32_t width;
    uint32_t height;
    int32_t projFormat;       // ERP or CUBE
    int32_t pixFormat; // RGB or YUV
    bool hasAudio;
    uint32_t audioChannel;
    uint32_t stride;
    uint32_t numberOfStreams; // number of video streams.
    uint32_t frameRate;
    int64_t duration;
    uint32_t frameNum;
    uint32_t currentFrameNum;
    uint32_t sourceNumber;
    struct SourceWH *sourceWH;
    //more information to do
};

struct SourceData
{
    AVFormatContext *fmt_ctx;
    int stream_idx;
    AVStream *video_stream;
    AVCodecContext *codec_ctx;
    AVCodec *decoder;
    AVPacket *packet;
    AVFrame *av_frame;
    AVFrame *gl_frame;
    struct SwsContext *conv_ctx;
};

struct Pose
{
    float yaw;
    float pitch;
};

struct SourceInfo
{
    uint32_t sourceWidth;
    uint32_t sourceHeight;
    uint32_t tileRowNumber;
    uint32_t tileColumnNumber;
};

struct RegionInfo
{
    uint32_t sourceNumber;
    RegionWisePacking *regionWisePacking;
    struct SourceInfo *sourceInfo;
};

struct BufferInfo
{
    uint8_t *buffer[4];
    uint32_t width;
    uint32_t height;
    uint32_t stride[4]; //rgb width*3
    PixelFormat::Enum pixelFormat;
};

struct MultiBufferInfo
{
    uint32_t bufferNumber;
    struct BufferInfo *bufferInfo;
};

struct RenderInfo
{
    uint32_t width;
    uint32_t height;
    uint32_t windowWidth;
    uint32_t windowHeight;
};

enum RenderStatus
{
    RENDER_ERROR = -1,

    RENDER_CREATE_ERROR,
    RENDER_STATUS_OK
};

enum DecoderStatus
{
    PACKET_ERROR = -1,

    FRAME_ERROR,
    DECODER_OK
};

#endif
