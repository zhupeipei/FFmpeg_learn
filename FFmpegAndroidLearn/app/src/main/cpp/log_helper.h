//
// Created by xmly on 2022/9/8.
//

#ifndef FFMPEGANDROIDLEARN_LOG_HELPER_H
#define FFMPEGANDROIDLEARN_LOG_HELPER_H

#endif //FFMPEGANDROIDLEARN_LOG_HELPER_H

#include "android/log.h"

#define LOGI(TAG, FMT, ...) __android_log_print(ANDROID_LOG_INFO, TAG, FMT, __VA_ARGS__)
#define LOGE(TAG, FMT, ...) __android_log_print(ANDROID_LOG_ERROR, TAG, FMT, __VA_ARGS__)
