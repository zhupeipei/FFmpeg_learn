//
// Created by xmly on 2022/9/8.
//

#ifndef FFMPEGANDROIDLEARN_SIMPLE_AUDIO_PLAYER_H
#define FFMPEGANDROIDLEARN_SIMPLE_AUDIO_PLAYER_H

#ifdef __cplusplus
extern "C" {
#endif
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
#ifdef __cplusplus
}
#endif

#include <jni.h>
#include "log_helper.h"
#include "unistd.h"

#define BUFFER_SIZE (48000 * 10)

class SimpleAudioPlayer {
private:
    AVFormatContext *avFormatContext;
    AVCodecContext *avCodecContext;
    const AVCodec *avCodec;
public:
    SwrContext *swrContext;
    AVPacket *avPacket;
    AVFrame *avFrame;

    int audio_index;

    uint8_t *out_buffer;

    int outSampleRateHz;
    AVChannelLayout *outChannelConfig;
    enum AVSampleFormat outAudioFormat;
    int outChannelCount;

    int open(const char *file);

    int decodeAudio();

    void *sendToAudioTrack();

    int close();
};

#endif //FFMPEGANDROIDLEARN_SIMPLE_AUDIO_PLAYER_H
