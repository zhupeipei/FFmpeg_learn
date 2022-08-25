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

static int g_frame_rate = 1;
static int g_sfp_refresh_thread_exit = 0;
static int g_sfp_refresh_thread_pause = 0;

#define SFM_REFRESH_EVENT (SDL_USEREVENT+1)
#define SFM_BREAK_EVENT (SDL_USEREVENT+2)

int sfp_refresh_thread(void *opaque) {
    g_sfp_refresh_thread_exit = 0;
    g_sfp_refresh_thread_pause = 0;
    while (!g_sfp_refresh_thread_exit) {
        if (!g_sfp_refresh_thread_pause) {
            SDL_Event sdlEvent;
            sdlEvent.type = SFM_REFRESH_EVENT;
            SDL_PushEvent(&sdlEvent);
        }
        SDL_Delay(1000 / g_frame_rate);
    }
    g_sfp_refresh_thread_exit = 0;
    g_sfp_refresh_thread_pause = 0;
    SDL_Event sdl_event;
    sdl_event.type = SFM_BREAK_EVENT;
    SDL_PushEvent(&sdl_event);
    return 0;
}

int main() {
    std::cout << "Hello, MyPlayer" << std::endl;

    int ret = 0;

    char *file_path = "/Users/xmly/audio_video/FFmpeg_learn/source/test.mp4";

    AVPixelFormat dstPixelFormat;

    // ffmpeg param
    AVFormatContext *avFormatContext;
    AVCodecContext *avCodecContext = NULL;
    const AVCodec *avCodec;
    AVPacket *avPacket = NULL;
    AVFrame *pFrame = NULL, *pFrameYUV = NULL;
    SwsContext *swsContext;
    int out_buffer_size = 0;
    unsigned char *output_buffer = 0;
    int video_index = 0;

    avFormatContext = avformat_alloc_context();
    swsContext = sws_alloc_context();

    // 1. 初始化网络库
//    avformat_network_init();

    // 2. 打开输入流
    int res = avformat_open_input(&avFormatContext, file_path, NULL, NULL);
    if (res != 0) {
        ret = -1;
        goto end;
    }

    // 3. 获取流信息
    res = avformat_find_stream_info(avFormatContext, NULL);
    if (res < 0) {
        ret = -1;
        goto end;
    }

    // 4. 查找解码器
    for (int i = 0; i < avFormatContext->nb_streams; i++) {
        AVStream *stream = avFormatContext->streams[i];
        if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_index = i;
            avCodec = avcodec_find_decoder(stream->codecpar->codec_id);
            avCodecContext = avcodec_alloc_context3(avCodec);
            avcodec_parameters_to_context(avCodecContext, stream->codecpar);
            // avcodec_parameters_from_context(stream->codecpar, avCodecContext);
            g_frame_rate = stream->avg_frame_rate.num / stream->avg_frame_rate.den;
            std::cout << "g_frame_rate: " << g_frame_rate << std::endl;
            break;
        }
    }
    if (avCodecContext == NULL) {
        ret = -1;
        goto end;
    }

    // 5. 打开解码器，open codec
    res = avcodec_open2(avCodecContext, avCodec, NULL);
    if (res != 0) {
        ret = -1;
        goto end;
    }

    dstPixelFormat = avCodecContext->pix_fmt; // AV_PIX_FMT_YUV420P
    std::cout << "as: " << avCodecContext->pix_fmt << ", " << AV_PIX_FMT_YUV420P << ", " << AV_PIX_FMT_YUYV422
              << std::endl;
    swsContext = sws_getContext(avCodecContext->width, avCodecContext->height,
                                avCodecContext->pix_fmt,
                                avCodecContext->width, avCodecContext->height,
                                dstPixelFormat,
                                SWS_BICUBIC,
                                NULL, NULL, NULL);

    // 打印日志
    av_dump_format(avFormatContext, video_index, file_path, 0);

    avPacket = av_packet_alloc();
    pFrame = av_frame_alloc();
    pFrameYUV = av_frame_alloc();

    out_buffer_size = av_image_get_buffer_size(AV_PIX_FMT_YUV420P,
                                               avCodecContext->width, avCodecContext->height, 1);
    output_buffer = (unsigned char *) (av_malloc(out_buffer_size));
    av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, output_buffer, avCodecContext->pix_fmt,
                         avCodecContext->width, avCodecContext->height, 1);

    // sdl param
//    SDL_Param *sdlParam = NULL;
    SDL_Window *sdlWindow;
    SDL_Renderer *sdlRenderer;
    SDL_Texture *sdlTexture;
    SDL_Rect sdlRect;
    SDL_Thread *sdlThread;

    // init sdl
    res = SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER);
    if (res != 0) {
        ret = -1;
        goto end;
    }
    sdlWindow = SDL_CreateWindow(file_path, 0, 0, avCodecContext->width, avCodecContext->height, 0);
    if (sdlWindow == NULL) {
        ret = -1;
        goto end;
    }
    sdlRenderer = SDL_CreateRenderer(sdlWindow, 0, 0);
    if (sdlRenderer == NULL) {
        ret = -1;
        goto end;
    }
    sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,
                                   avCodecContext->width, avCodecContext->height);
    if (sdlTexture == NULL) {
        ret = -1;
        goto end;
    }
    sdlRect.x = 0;
    sdlRect.y = 0;
    sdlRect.w = avCodecContext->width;
    sdlRect.h = avCodecContext->height;

    sdlThread = SDL_CreateThread(sfp_refresh_thread, "sdl_thread_zzz", NULL);

    // demuxing and show
    SDL_Event sdlEvent;
    while (true) {
        int temp_ret = 0;
        int wait_ret = SDL_WaitEvent(&sdlEvent);
        if (sdlEvent.type == SFM_REFRESH_EVENT) {
            while (true) {
                if (av_read_frame(avFormatContext, avPacket) < 0) {
                    g_sfp_refresh_thread_exit = 1;
                    break;
                }
                if (avPacket->stream_index == video_index) {
                    break;
                }
            }
            if (avcodec_send_packet(avCodecContext, avPacket)) {
                g_sfp_refresh_thread_exit = 1;
            }
            do {
                temp_ret = avcodec_receive_frame(avCodecContext, pFrame);
                if (temp_ret == AVERROR_EOF) {
                    g_sfp_refresh_thread_exit = 1;
                    break;
                } else if (temp_ret == 0) {
                    sws_scale(swsContext, pFrame->data, pFrame->linesize,
                              0, avCodecContext->height,
                              pFrameYUV->data, pFrameYUV->linesize);
                    SDL_UpdateTexture(sdlTexture, &sdlRect, pFrameYUV->data[0], pFrameYUV->linesize[0]);
                    SDL_RenderClear(sdlRenderer);
                    SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &sdlRect);
                    SDL_RenderPresent(sdlRenderer);
                }
            } while (temp_ret != AVERROR(EAGAIN));
        } else if (sdlEvent.type == SDL_QUIT) {
            std::cout << "SDL_QUIT" << std::endl;
            g_sfp_refresh_thread_exit = 1;
        } else if (sdlEvent.type == SDL_KEYDOWN) {
            std::cout << "SDL_KEYDOWN" << std::endl;
            if (sdlEvent.key.keysym.sym == SDLK_SPACE) {
                g_sfp_refresh_thread_pause = !g_sfp_refresh_thread_pause;
            } else if (sdlEvent.key.keysym.sym == SDLK_q) {
                g_sfp_refresh_thread_exit = 1;
            }
        } else if (sdlEvent.type == SFM_BREAK_EVENT) {
            std::cout << "SFM_BREAK_EVENT" << std::endl;
            break;
        }
    }

    end:
    {
        // release ffmpeg
        avformat_close_input(&avFormatContext);
        avformat_free_context(avFormatContext);
        av_packet_free(&avPacket);
        av_frame_free(&pFrame);
        av_frame_free(&pFrameYUV);
        sws_freeContext(swsContext);
        if (avCodecContext != NULL) {
            avcodec_close(avCodecContext);
            avcodec_free_context(&avCodecContext);
        }

        // release sdl
        SDL_DestroyWindow(sdlWindow);
        SDL_DestroyRenderer(sdlRenderer);
        SDL_DestroyTexture(sdlTexture);
        SDL_Quit();
    }

    return ret;
}
