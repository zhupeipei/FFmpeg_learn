#!/bin/bash

#动态链接库，主要就是enable-shared，这里会生成独立的so库
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
  ANDROID=android
  CFLAGS=""
  LDFLAGS=""
else
  echo "build for 32bit"
  ARCH=arm
  CPU=armv7-a
  API=29
  PLATFORM=armv7a
  ANDROID=androideabi
  CFLAGS="-mfloat-abi=softfp -march=$CPU"
  LDFLAGS="-Wl,--fix-contex-a8"
fi

export NDK=/Users/xmly/Library/Android/sdk/ndk/21.4.7075529
export TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/darwin-x86_64/bin
export SYSROOT=$NDK/toolchains/llvm/prebuilt/darwin-x86_64/sysroot

export CROSS_PREFIX=$TOOLCHAIN/$ARCH-linux-$ANDROID-
export CC=$TOOLCHAIN/$PLATFORM-linux-$ANDROID$API-clang
export CXX=$TOOLCHAIN/$PLATFORM-linux-$ANDROID$API-clang++
export PREFIX=/Users/xmly/audio_video/FFmpeg_compile_local_mac/android/build/$CPU

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
    --disable-static \
    --enable-shared \
    --disable-ffmpeg \
    --disable-ffplay \
    --disable-ffprobe \
    --disable-debug \
    --disable-doc \
    --disable-avdevice \
    --disable-postproc \
    --disable-encoders \
    --disable-decoders \
    $ADDITIONAL_CONFIGURE_FLAG

    make -j8
    make install
}

build_ffmpeg_android
