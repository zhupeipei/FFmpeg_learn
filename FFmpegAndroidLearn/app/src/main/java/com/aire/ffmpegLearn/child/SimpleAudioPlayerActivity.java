package com.aire.ffmpegLearn.child;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.os.Bundle;
import android.os.Environment;

import com.aire.ffmpegLearn.R;
import com.aire.ffmpegLearn.databinding.ActivitySimpleAudioPlayerBinding;
import com.aire.ffmpegLearn.media.SimpleAudioPlayer;
import com.easypermission.EasyPermission;
import com.easypermission.GrantResult;
import com.easypermission.PermissionRequestListener;
import com.easypermission.RequestPermissionRationalListener;

import java.io.File;
import java.util.Map;
import java.util.concurrent.Executors;

public class SimpleAudioPlayerActivity extends AppCompatActivity {
    private static final String TAG = "SimpleAudioPlayerActivity";
    private ActivitySimpleAudioPlayerBinding mAudioPlayerBinding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mAudioPlayerBinding = ActivitySimpleAudioPlayerBinding.inflate(getLayoutInflater());
        setContentView(mAudioPlayerBinding.getRoot());

        EasyPermission.with(this)
                .addPermission(Manifest.permission.READ_EXTERNAL_STORAGE)
                .addPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE)
                .request(new PermissionRequestListener() {
                    @Override
                    public void onGrant(Map<String, GrantResult> result) {

                    }

                    @Override
                    public void onCancel(String stopPermission) {

                    }
                });

        mAudioPlayerBinding.playAudioBtn.setOnClickListener(v -> {
            Executors.newSingleThreadExecutor().submit(new Runnable() {
                @Override
                public void run() {
                    if (!EasyPermission.isPermissionGrant(
                            SimpleAudioPlayerActivity.this,
                            Manifest.permission.READ_EXTERNAL_STORAGE,
                            Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
                        return;
                    }
                    File folder = new File(Environment.getExternalStorageDirectory(), "/sdcard/test");
                    String filePath = new File(folder, "audio.m4a").getAbsolutePath();
//                    String filePath = new File(folder, "audio_record.wav").getAbsolutePath();
                    new SimpleAudioPlayer().playAudio(filePath);
                }
            });
        });
    }
}