//
// Created by xmly on 2022/9/8.
//

#include "simple_audio_player.h"

#define SLEEP_TIME (16000)

#define SIMPLE_AUDIO_PLAYER_TAG "simple_audio_player"

extern "C"
JNIEXPORT void JNICALL
Java_com_aire_ffmpegLearn_media_SimpleAudioPlayer_playAudio(JNIEnv *env,
                                                            jobject thiz,
                                                            jstring path) {
    if (!path) {
        return;
    }
    const char *native_path = env->GetStringUTFChars(path, JNI_FALSE);
    if (native_path == nullptr) {
        return;
    }
    jclass clazz = nullptr;
    jobject audio_track = nullptr;
    jmethodID newAudioTrackMethodId = nullptr;
    jmethodID writeAudioDataMethodId = nullptr;
    jmethodID releaseAudioDataMethodId = nullptr;
    jmethodID playMethodId = nullptr;
    jclass audio_track_clazz = nullptr;

    SimpleAudioPlayer *player = new SimpleAudioPlayer;
    // 打开输入流
    int ret = player->open(native_path);
    if (ret < 0) {
        goto end;
    }

    clazz = env->FindClass("com/aire/ffmpegLearn/media/SimpleAudioPlayer");
    // 创建AudioTrack
    newAudioTrackMethodId = env->GetMethodID(clazz, "newAudioTrack",
                                             "(II)Landroid/media/AudioTrack;");
    audio_track = env->CallObjectMethod(thiz, newAudioTrackMethodId,
                                        player->outSampleRateHz, player->outChannelCount);
    // 调用audioTrack的play方法
    audio_track_clazz = env->GetObjectClass(audio_track);
    playMethodId = env->GetMethodID(audio_track_clazz, "play", "()V");
    env->CallVoidMethod(audio_track, playMethodId);
    // write audioData 方法
    writeAudioDataMethodId = env->GetMethodID(clazz, "writeAudioData", "([BI)V");
    // release audioTrack方法
    releaseAudioDataMethodId = env->GetMethodID(clazz, "releaseAudioTrack", "()V");

    player->avPacket = av_packet_alloc();
    player->avFrame = av_frame_alloc();

    while (ret >= 0) {
        // decode audio packet
        ret = player->decodeAudio();
        if (ret == 0) {
            continue;
        } else if (ret < 0) {
            break;
        }
        int size = ret;
        // 调用AudioTrack播放（可优化：数组复用）
        jbyteArray data = env->NewByteArray(size);
        jbyte *arr_data = env->GetByteArrayElements(data, JNI_FALSE);
        memcpy(arr_data, player->out_buffer, size);
        env->ReleaseByteArrayElements(data, arr_data, 0);
        env->CallVoidMethod(thiz, writeAudioDataMethodId, data, size);
        env->DeleteLocalRef(data);

        // 延时等待
        usleep(SLEEP_TIME);
    }

    end:
    env->CallVoidMethod(thiz, releaseAudioDataMethodId);
    player->close();
    delete player;
    env->ReleaseStringUTFChars(path, native_path);
}

int SimpleAudioPlayer::open(const char *file) {
    LOGI(SIMPLE_AUDIO_PLAYER_TAG, "open file: %s", file);
    if (!file) {
        return -1;
    }
    avFormatContext = avformat_alloc_context();
    int ret = avformat_open_input(&avFormatContext, file, nullptr, nullptr);
    if (ret < 0) {
        LOGE(SIMPLE_AUDIO_PLAYER_TAG, "avformat_open_input error: %s", av_err2str(ret));
        return ret;
    }
    ret = avformat_find_stream_info(avFormatContext, nullptr);
    if (ret < 0) {
        LOGE(SIMPLE_AUDIO_PLAYER_TAG, "avformat_find_stream_info error: %s", av_err2str(ret));
        return ret;
    }
    int stream_count = avFormatContext->nb_streams;
    for (int i = 0; i < stream_count; i++) {
        AVStream *stream = avFormatContext->streams[i];
        if (stream->codecpar && stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_index = i;

            av_dump_format(avFormatContext, i, file, 0);

            LOGI(SIMPLE_AUDIO_PLAYER_TAG, "found audio stream, index=%d", audio_index);

            avCodec = avcodec_find_decoder(stream->codecpar->codec_id);
            if (!avCodec) {
                LOGE(SIMPLE_AUDIO_PLAYER_TAG, "avcodec_find_decoder error", "");
                return -1;
            }
            avCodecContext = avcodec_alloc_context3(avCodec);
            ret = avcodec_parameters_to_context(avCodecContext, stream->codecpar);
            if (ret < 0) {
                LOGE(SIMPLE_AUDIO_PLAYER_TAG, "avcodec_parameters_to_context error %s",
                     av_err2str(ret));
                return ret;
            }
            ret = avcodec_open2(avCodecContext, avCodec, nullptr);
            if (ret < 0) {
                LOGE(SIMPLE_AUDIO_PLAYER_TAG, "avcodec_open2 error %s", av_err2str(ret));
                return ret;
            }
            break;
        }
    }
    if (!avCodec) {
        return -1;
    }
    int in_sample_rate = avCodecContext->sample_rate;
    AVChannelLayout &in_ch_layout = avCodecContext->ch_layout;
    AVSampleFormat in_sample_fmt = avCodecContext->sample_fmt;
    LOGI(SIMPLE_AUDIO_PLAYER_TAG,
         "in_sample_rate: %d, in_ch_layout.nb_channels: %d, in_ch_layout.mask : %lu, in_sample_fmt: %d",
         in_sample_rate, in_ch_layout.nb_channels, in_ch_layout.u.mask, in_sample_fmt);

    outSampleRateHz = avCodecContext->sample_rate;
    outChannelConfig = &avCodecContext->ch_layout;
    outAudioFormat = AV_SAMPLE_FMT_S16;// avCodecContext->sample_fmt;
    outChannelCount = avCodecContext->ch_layout.nb_channels;
    LOGI(SIMPLE_AUDIO_PLAYER_TAG,
         "outSampleRateHz: %d, outChannelConfig.nb_channels: %d, outChannelConfig.mask : %lu, outAudioFormat: %d",
         outSampleRateHz, outChannelConfig->nb_channels, outChannelConfig->u.mask, outAudioFormat);

    swrContext = swr_alloc();
    ret = swr_alloc_set_opts2(&swrContext,
                              outChannelConfig, outAudioFormat,
                              outSampleRateHz,
                              &in_ch_layout, in_sample_fmt,
                              in_sample_rate,
                              0, nullptr);
    if (ret < 0) {
        LOGE(SIMPLE_AUDIO_PLAYER_TAG, "swr_alloc_set_opts2 error %s", av_err2str(ret));
        return ret;
    }
    ret = swr_init(swrContext);
    if (ret < 0) {
        LOGE(SIMPLE_AUDIO_PLAYER_TAG, "swr_init error %s", av_err2str(ret));
        return ret;
    }
    out_buffer = new uint8_t[BUFFER_SIZE];
    return ret;
}

int SimpleAudioPlayer::decodeAudio() {
    int ret = av_read_frame(avFormatContext, avPacket);
    if (ret < 0) {
        LOGE(SIMPLE_AUDIO_PLAYER_TAG, "av_read_frame error %s", av_err2str(ret));
        return ret;
    }
    if (avPacket->stream_index != audio_index) {
        LOGE(SIMPLE_AUDIO_PLAYER_TAG, "avPacket stream_index not audio packet", "");
        return -1;
    }
    ret = avcodec_send_packet(avCodecContext, avPacket);
    if (ret < 0) {
        LOGE(SIMPLE_AUDIO_PLAYER_TAG, "avcodec_send_packet error %s", av_err2str(ret));
        return ret;
    }
    ret = avcodec_receive_frame(avCodecContext, avFrame);
    if (ret < 0) {
        LOGE(SIMPLE_AUDIO_PLAYER_TAG, "avcodec_receive_frame error %s", av_err2str(ret));
        if (ret == AVERROR(EAGAIN)) {
            return 0; // 这里需要输入新的数据
        }
        return ret;
    }
    // 音频格式转换
    ret = swr_convert(swrContext, &out_buffer, BUFFER_SIZE,
                      (const uint8_t **) avFrame->data, avFrame->nb_samples);
    if (ret < 0) {
        LOGE(SIMPLE_AUDIO_PLAYER_TAG, "swr_convert error %s", av_err2str(ret));
        return ret;
    }
    // 获取转换后的缓冲区大小
    int buffer_size = av_samples_get_buffer_size(avFrame->linesize, outChannelCount,
                                                 avFrame->nb_samples, outAudioFormat, 1);
    LOGI(SIMPLE_AUDIO_PLAYER_TAG, "buffer_size=%d", buffer_size);
    av_frame_unref(avFrame);
    av_packet_unref(avPacket);
    return buffer_size;
}

int SimpleAudioPlayer::close() {
    if (avFormatContext) {
        avformat_close_input(&avFormatContext);
        avformat_free_context(avFormatContext);
    }
    if (avCodecContext) {
        avcodec_close(avCodecContext);
        avcodec_free_context(&avCodecContext);
    }
    if (avPacket) {
        av_packet_free(&avPacket);
    }
    if (avFrame) {
        av_frame_free(&avFrame);
    }
    if (swrContext) {
        swr_close(swrContext);
        swr_free(&swrContext);
    }
    delete[] out_buffer;
    return 0;
}