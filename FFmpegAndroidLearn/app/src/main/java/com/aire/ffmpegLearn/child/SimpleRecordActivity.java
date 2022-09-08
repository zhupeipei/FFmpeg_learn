package com.aire.ffmpegLearn.child;

import android.Manifest;
import android.content.pm.PackageManager;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import com.aire.ffmpegLearn.databinding.ActivitySimpleRecordBinding;
import com.easypermission.EasyPermission;
import com.easypermission.GrantResult;
import com.easypermission.NextAction;
import com.easypermission.NextActionType;
import com.easypermission.PermissionRequestListener;
import com.easypermission.RequestPermissionRationalListener;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Map;

public class SimpleRecordActivity extends AppCompatActivity {
    private static final String TAG = "SimpleRecordActivity";

    private ActivitySimpleRecordBinding mSimpleRecordBinding;
    private AudioRecord mAudioRecord;
    private RecordThread mRecordThread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mSimpleRecordBinding = ActivitySimpleRecordBinding.inflate(getLayoutInflater());
        setContentView(mSimpleRecordBinding.getRoot());

        RequestPermissionRationalListener requestPermissionRationalListener = new RequestPermissionRationalListener() {
            @Override
            public void onRequestPermissionRational(String permission, boolean requestPermissionRationaleResult, NextAction nextAction) {
                Log.i(TAG, "onRequestPermissionRational permission: " + permission
                        + ", requestPermissionRationaleResult: " + requestPermissionRationaleResult
                        + ", nextAction: " + nextAction);
                if (nextAction != null) {
                    nextAction.next(NextActionType.NEXT);
                }
            }
        };

        EasyPermission.with(this)
                .addPermission(Manifest.permission.RECORD_AUDIO)
                .addPermission(Manifest.permission.READ_EXTERNAL_STORAGE)
                .addPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE)
                .addRequestPermissionRationaleHandler(
                        Manifest.permission.READ_EXTERNAL_STORAGE, requestPermissionRationalListener)
                .addRequestPermissionRationaleHandler(
                        Manifest.permission.WRITE_EXTERNAL_STORAGE, requestPermissionRationalListener)
                .request(new PermissionRequestListener() {
                    @Override
                    public void onGrant(Map<String, GrantResult> result) {
                        Log.i(TAG, "onGrant: ");
                    }

                    @Override
                    public void onCancel(String stopPermission) {
                        Log.i(TAG, "onCancel: ");
                    }
                });

        // ffmpeg -f s16le -ar 44100 -ac 2 -i audio_record.pcm out.wav
        mSimpleRecordBinding.startRecord.setOnClickListener(v -> {
            boolean granted = EasyPermission.isPermissionGrant(SimpleRecordActivity.this, Manifest.permission.RECORD_AUDIO);
            if (!granted) {
                return;
            }
            Log.i(TAG, "startRecord start");
            mRecordThread = new RecordThread();
            mRecordThread.start();
            mSimpleRecordBinding.recordStatus.setText("开始录音\n", TextView.BufferType.NORMAL);
        });

        mSimpleRecordBinding.stopRecord.setOnClickListener(v -> {
            Log.i(TAG, "stopRecord end");
            // 停止线程
            if (mRecordThread != null && !mRecordThread.isInterrupted()) {
                mRecordThread.mStopRecording = true;
                mRecordThread.interrupt();
                mRecordThread = null;
            }

            // 停止录音
            if (mAudioRecord != null) {
                mAudioRecord.stop();
                mAudioRecord.release();
                mAudioRecord = null;
            }
            mSimpleRecordBinding.recordStatus.append("结束录音\n");
        });
    }

    private class RecordThread extends Thread {
        private File mFilePath = null;
        private String mFileName = "audio_record.pcm";

        private volatile boolean mStopRecording = false;

        public RecordThread() {
            File folder = new File(Environment.getExternalStorageDirectory(), "/sdcard/test");
            try {
                folder.mkdirs();

                mFilePath = new File(folder, mFileName);
                mFilePath.createNewFile();
            } catch (Exception e) {
                e.printStackTrace();
            }
            Log.i(TAG, "RecordTask record path: " + mFilePath);
        }

        @Override
        public void run() {
            int audioSource = MediaRecorder.AudioSource.MIC;
            int sampleRate = 44100;
            int channelConfig = AudioFormat.CHANNEL_IN_STEREO;
            int audioFormat = AudioFormat.ENCODING_PCM_16BIT;
            int bufferSize = AudioRecord.getMinBufferSize(sampleRate, channelConfig, audioFormat);
            if (ActivityCompat.checkSelfPermission(SimpleRecordActivity.this,
                    Manifest.permission.RECORD_AUDIO) != PackageManager.PERMISSION_GRANTED) {
                return;
            }
            mAudioRecord = new AudioRecord(audioSource, sampleRate, channelConfig, audioFormat, bufferSize);
            mAudioRecord.startRecording();
            int bufferAudioDataSize = 1024;
            byte[] audioData = new byte[bufferAudioDataSize];
            int len = 0;
            while (!mStopRecording) {
                len = mAudioRecord.read(audioData, 0, bufferAudioDataSize);
                if (len < 0) {
                    throw new RuntimeException("录音失败");
                }
                if (len == 0) {
                    continue;
                }
                writeDataToFile(audioData, 0, len);
            }
            try {
                mBufferedOutputStream.flush();
            } catch (IOException e) {
                e.printStackTrace();
            }
            closeOutputStream();
        }

        private BufferedOutputStream mBufferedOutputStream;

        private void writeDataToFile(byte[] data, int pos, int len) {
            if (mBufferedOutputStream == null) {
                try {
                    mBufferedOutputStream = new BufferedOutputStream(new FileOutputStream(mFilePath));
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                    closeOutputStream();
                }
            }
            if (mBufferedOutputStream == null) {
                return;
            }
            try {
                mBufferedOutputStream.write(data, pos, len);
            } catch (IOException e) {
                e.printStackTrace();
                closeOutputStream();
            }
        }

        private void closeOutputStream() {
            try {
                if (mBufferedOutputStream != null) {
                    mBufferedOutputStream.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}