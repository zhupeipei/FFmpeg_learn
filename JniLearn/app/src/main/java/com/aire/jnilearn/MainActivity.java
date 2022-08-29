package com.aire.jnilearn;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.aire.jnilearn.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    private ActivityMainBinding binding;

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
    }
}