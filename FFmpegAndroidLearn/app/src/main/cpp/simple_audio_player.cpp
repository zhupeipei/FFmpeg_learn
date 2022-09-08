//
// Created by xmly on 2022/9/8.
//

#include "simple_audio_player.h"
#include <jni.h>
#include "log_helper.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_aire_ffmpegLearn_media_SimpleAudioPlayer_playAudio(JNIEnv *env,
                                                            jobject thiz,
                                                            jstring path) {
    const char *native_path = env->GetStringUTFChars(path, JNI_FALSE);
    if (native_path == nullptr) {
        return;
    }
    SimpleAudioPlayer *player = new SimpleAudioPlayer;
    player->open(native_path);
    delete player;
    env->ReleaseStringUTFChars(path, native_path);
}

int SimpleAudioPlayer::open(const char *file) {
    LOGI("simple_audio_player", "open file: %s", file);
    return 1;
}

int SimpleAudioPlayer::decodeAudio() {
    return 0;
}

int SimpleAudioPlayer::close() {
    return 0;
}