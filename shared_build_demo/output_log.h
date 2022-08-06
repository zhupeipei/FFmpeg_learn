//
// Created by xmly on 2022/8/5.
//

#ifndef FFMPEG_CLION_PROJECT_OUTPUT_LOG_H
#define FFMPEG_CLION_PROJECT_OUTPUT_LOG_H

enum LOG_LEVEL
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

void output_log(LOG_LEVEL log_level, const char* fmt, ...);

//class output_log {
//
//};


#endif //FFMPEG_CLION_PROJECT_OUTPUT_LOG_H
