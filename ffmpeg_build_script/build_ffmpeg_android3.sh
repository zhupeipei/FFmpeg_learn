#!/bin/bash

#生成独立的so库，需要先生成静态库，然后链接起来

make distclean
make clean
set -e

archbit=64

if [ $archbit -eq 64 ];then
  echo "build for 64bit"
  ARCH=aarch64
  CPU=armv8-a
  API=29
  PLATFORM=aarch64
  PLATFORM_ARCH=arm64
  ANDROID=android
  CFLAGS=""
  LDFLAGS=""
else
  echo "build for 32bit"
  ARCH=arm
  CPU=armv7-a
  API=29
  PLATFORM=armv7a
  PLATFORM_ARCH=arm
  ANDROID=androideabi
  CFLAGS="-mfloat-abi=softfp -march=$CPU"
  LDFLAGS="-Wl,--fix-contex-a8"
fi

export NDK=/Users/xmly/Library/Android/sdk/ndk/21.4.7075529
export TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/darwin-x86_64/bin
export TOOL_PREFIX=$NDK/toolchains/llvm/prebuilt/darwin-x86_64
export SYSROOT=$NDK/toolchains/llvm/prebuilt/darwin-x86_64/sysroot

export CROSS_PREFIX=$TOOLCHAIN/$ARCH-linux-$ANDROID-
export CC=$TOOLCHAIN/$PLATFORM-linux-$ANDROID$API-clang
export CXX=$TOOLCHAIN/$PLATFORM-linux-$ANDROID$API-clang++
export PREFIX=/Users/xmly/audio_video/FFmpeg_compile_local_mac/android/build/$CPU

export PLATFORM_API=$NDK/platforms/android-$API/arch-$PLATFORM_ARCH

export H264_PREFIX=/Users/xmly/audio_video/FFmpeg_compile_local_mac/libx264/build/armv8-a
export CFLAGS="-I$H264_PREFIX/include"
export LDFLAGS="-L$H264_PREFIX/lib"
# 这里设置了PKG_CONFIG也没什么用
export PKG_CONFIG_PATH=$H264_PREFIX/lib/pkgconfig:$PKG_CONFIG_PATH

pkg-config --cflags x264

function build_ffmpeg_android() {
    ./configure \
    --prefix=$PREFIX \
    --cross-prefix=$CROSS_PREFIX \
    --target-os=android \
    --arch=$ARCH \
    --cpu=$CPU \
    --cc=$CC \
    --cxx=$CXX \
    --nm=$TOOLCHAIN/$ARCH-linux-$ANDROID-nm \
    --strip=$TOOLCHAIN/$ARCH-linux-$ANDROID-strip \
    --enable-cross-compile \
    --sysroot=$SYSROOT \
    --extra-cflags="$CFLAGS" \
    --extra-ldflags="$LDFLAGS" \
    --enable-static \
    --disable-shared \
    --disable-ffmpeg \
    --disable-ffplay \
    --disable-ffprobe \
    --disable-debug \
    --disable-doc \
    --disable-avdevice \
    --disable-postproc \
    --disable-encoders \
    --disable-decoders \
    --enable-gpl \
    --enable-libx264 \
    --enable-encoder=opus,mpeg4,aac,libx264 \
    --enable-decoder=h264,hevc,mpeg4,mp3,aac \
    $ADDITIONAL_CONFIGURE_FLAG

    make -j8
    make install
}

build_ffmpeg_android

function link_ffmpeg() {
    $TOOLCHAIN/$ARCH-linux-$ANDROID-ld -rpath-link=PLATFORM_API/usr/lib -L$PLATFORM_API/usr/lib \
        -L$PREFIX/lib -soname libffmpeg.so \
        -shared -Bsymbolic --whole-archive --no-undefined -o $PREFIX/libffmpeg.so \
        $PREFIX/lib/libavutil.a \
        $PREFIX/lib/libavformat.a \
        $PREFIX/lib/libavcodec.a \
        $PREFIX/lib/libswscale.a \
        $PREFIX/lib/libswresample.a \
        $PREFIX/lib/libavfilter.a \
        $H264_PREFIX/lib/libx264.a \
        -lstdc++ -lc -lm -lz -ldl -llog --dynamic-linker=/system/bin/linker $TOOL_PREFIX/lib/gcc/$ARCH-linux-$ANDROID/4.9.x/libgcc_real.a
}

link_ffmpeg