package com.aire.ffmpegLearn.media;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.util.Log;

/**
 * @author ZhuPeipei
 * @date 2022/9/8 21:40
 */
public class SimpleAudioPlayer {
    private static final String TAG = "SimpleAudioPlayer";

    static {
        Log.i(TAG, "static initializer: ");
        System.loadLibrary("ffmpegLearn");
    }

    private AudioTrack mAudioTrack = null;

    public native void playAudio(String file);

    public AudioTrack newAudioTrack(int sampleRateHz, int channelCount) {
        if (mAudioTrack != null) {
            return mAudioTrack;
        }
        Log.i(TAG, "newAudioTrack sampleRateHz: " + sampleRateHz + ", channelCount: " + channelCount);
        int audioFormat = AudioFormat.ENCODING_PCM_16BIT;
        int channelConfig = channelCount == 1 ? AudioFormat.CHANNEL_OUT_MONO : AudioFormat.CHANNEL_OUT_STEREO;
        int bufferSizeInBytes = AudioTrack.getMinBufferSize(sampleRateHz, channelConfig, audioFormat);
        mAudioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRateHz, channelConfig,
                audioFormat, bufferSizeInBytes, AudioTrack.MODE_STREAM);
        return mAudioTrack;
    }

    public void releaseAudioTrack() {
        Log.i(TAG, "releaseAudioTrack: ");
        if (mAudioTrack != null) {
            mAudioTrack.stop();
            mAudioTrack.release();
            mAudioTrack = null;
        }
    }

    public void writeAudioData(byte[] audioData, int len) {
        Log.i(TAG, "writeAudioData: " + (audioData != null ? audioData.length : -1) + ", len: " + len);
        if (mAudioTrack != null && audioData != null) {
            mAudioTrack.write(audioData, 0, len);
        }
    }
}
