#include <iostream>

extern "C" {
#include "libavutil/avstring.h"
#include "libavutil/channel_layout.h"
#include "libavutil/eval.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/imgutils.h"
#include "libavutil/dict.h"
#include "libavutil/fifo.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/time.h"
#include "libavutil/bprint.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
#include "libavcodec/avfft.h"
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"
}

#include "SDL.h"
#include "SDL_thread.h"

#include "output_log.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    // ffmpeg param
    AVFormatContext *avFormatContext;
    AVCodecContext *avCodecContext;
    AVCodec *avCodec;
    AVPacket *avPacket = NULL;
    AVFrame *pFrame = NULL, *pFrameYUV = NULL;
    SwsContext *swsContext;
    int out_buffer_size = 0;
    unsigned char* output_buffer = 0;
    int video_index = 0;

    // sdl param
//    SDL_Param *sdlParam = NULL;
    SDL_Window *sdlWindow;
    SDL_Renderer *sdlRenderer;
    SDL_Texture *sdlTexture;
    SDL_Rect *sdlRect;
    SDL_Thread *sdlThread;
    SDL_Event sdlEvent;

    int ret = 0;

    // init ffmpeg
//    ffmpegVParam = new Ffmpeg_V_Param();
//    memset(ffmpegVParam, 0, sizeof(Ffmpeg_V_Param)); // 初始化
//     init_ffmpeg(ffmpegVParam, "./source/test.mp4");
    avFormatContext = avformat_alloc_context();
    avPacket = av_packet_alloc();
    pFrame = av_frame_alloc();
    pFrameYUV = av_frame_alloc();

    // init sdl



    // show

    // release

    return 0;
}
