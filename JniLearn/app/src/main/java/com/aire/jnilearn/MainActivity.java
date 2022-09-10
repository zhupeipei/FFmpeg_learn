package com.aire.jnilearn;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.aire.jnilearn.databinding.ActivityMainBinding;

import java.util.Arrays;

public class MainActivity extends AppCompatActivity {

    private ActivityMainBinding binding;
    private String str = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        NdkHelper ndkHelper = new NdkHelper();
        String jniStr = ndkHelper.stringFromJNI();
        Log.i("ndkLearn", "onCreate: " + jniStr);

        tv.setText(jniStr);

        int[] arr = {1, 2, 3, 4, 5};
        ndkHelper.setIntData(arr);

        // ndk callback
        ndkHelper.trigger("hello jni");

        // ndk 修改数组中的值
        int[] arr1 = {1,2,3,4};
        Log.i("ndkLearn", "onCreate 1: " + Arrays.toString(arr1));
        ndkHelper.modifyArr(arr1);
        Log.i("ndkLearn", "onCreate 2: " + Arrays.toString(arr1));

        // ndk 修改string中的值
        // 这里 无法实现
        str = "I'm from java world";
        ndkHelper.modifyString(str);
        Log.i("ndkLearn", "onCreate: " + str);
    }
}