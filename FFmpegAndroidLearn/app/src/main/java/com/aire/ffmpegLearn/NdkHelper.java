package com.aire.ffmpegLearn;

/**
 * @author ZhuPeipei
 * @date 2022/9/8 10:22
 */
public class NdkHelper {
    // Used to load the 'ffmpegLearn' library on application startup.
    static {
        System.loadLibrary("ffmpegLearn");
    }

    /**
     * A native method that is implemented by the 'ffmpegLearn' native library,
     * which is packaged with this application.
     */
    public native String ffmpegVersion();
}
