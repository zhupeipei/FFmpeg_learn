package com.aire.ffmpegLearn;

import android.util.Log;

/**
 * @author ZhuPeipei
 * @date 2022/9/8 10:22
 */
public class NdkHelper {
    private static final String TAG = "NdkHelper";

    // Used to load the 'ffmpegLearn' library on application startup.
    static {
        Log.i(TAG, "static initializer: ");
        System.loadLibrary("ffmpegLearn");
    }

    /**
     * A native method that is implemented by the 'ffmpegLearn' native library,
     * which is packaged with this application.
     */
    public native String ffmpegVersion();
}
