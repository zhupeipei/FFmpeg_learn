#include <jni.h>
#include <string>
#include "android/log.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "libavutil/ffversion.h"
#ifdef __cplusplus
}
#endif

#define LOGV(FMT, ...) __android_log_print(ANDROID_LOG_VERBOSE, "nativeLog", FMT, __VA_ARGS__)

extern "C"
JNIEXPORT jstring JNICALL
Java_com_aire_ffmpegLearn_NdkHelper_ffmpegVersion(JNIEnv *env, jobject thiz) {
    std::string hello = "Hello from C++";
    LOGV("FFMPEG VERSION: %s", FFMPEG_VERSION);
    LOGV("AVCODEC VERSION: %d", avcodec_version());
    const AVCodec *codec = avcodec_find_encoder_by_name("libx264");
    if (codec) {
        LOGV("FOUND X264, name: %s, type: %d", codec->name, codec->type);
    }
    return env->NewStringUTF(hello.c_str());
}