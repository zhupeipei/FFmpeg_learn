package com.aire.ffmpegLearn.media;

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

    public native void playAudio(String file);
}
