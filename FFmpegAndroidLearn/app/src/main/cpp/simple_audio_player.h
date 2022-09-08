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
#ifdef __cplusplus
}
#endif

class SimpleAudioPlayer {
private:
public:
    int open(const char *file);

    int decodeAudio();

    int close();
};

#endif //FFMPEGANDROIDLEARN_SIMPLE_AUDIO_PLAYER_H
