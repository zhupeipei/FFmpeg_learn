package com.aire.ffmpegLearn;

import android.content.Intent;
import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

import com.aire.ffmpegLearn.child.SimpleRecordActivity;
import com.aire.ffmpegLearn.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        binding.recordTv.setOnClickListener(v -> {
            startActivity(new Intent(MainActivity.this, SimpleRecordActivity.class));
        });
    }
}