package com.aire.jnilearn;

import android.util.Log;

/**
 * @author ZhuPeipei
 * @date 2022/8/29 21:57
 */
public class NdkHelper {
    private static final String TAG = "ndkHelper";

    // Used to load the 'jnilearn' library on application startup.
    static {
        System.loadLibrary("jnilearn");
    }

    /**
     * A native method that is implemented by the 'jnilearn' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void setIntData(int[] data);

    void onJniCallback(String msg) {
        if (msg != null) {
            Log.i(TAG, "onJniCallback: " + msg);
        }
    }

    public native void trigger(String msg);

    public native void modifyArr(int[] arr);

    public native void modifyString(String value);
}
