#!/bin/bash

ARCH=aarch64
CPU=armv8-a
API=29
PLATFORM=aarch64
PLATFORM_ARCH=arm64
ANDROID=android
CFLAGS=""
LDFLAGS=""

export NDK=/Users/xmly/Library/Android/sdk/ndk/21.4.7075529
export TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/darwin-x86_64/bin
export TOOL_PREFIX=$NDK/toolchains/llvm/prebuilt/darwin-x86_64
export SYSROOT=$NDK/toolchains/llvm/prebuilt/darwin-x86_64/sysroot

export CROSS_PREFIX=$TOOLCHAIN/$ARCH-linux-$ANDROID-
export CC=$TOOLCHAIN/$PLATFORM-linux-$ANDROID$API-clang
export CXX=$TOOLCHAIN/$PLATFORM-linux-$ANDROID$API-clang++
export PREFIX=/Users/xmly/audio_video/FFmpeg_compile_local_mac/libx264/build/$CPU
export HOST=aarch64-linux-android

function build_x264()
{
echo "Compiling x264 for $ANDROID_ABI"
./configure \
    --prefix=$PREFIX \
    --disable-asm \
    --enable-static \
    --enable-shared \
    --enable-pic \
    --host=$HOST \
    --cross-prefix=$CROSS_PREFIX \
    --sysroot=$SYSROOT \

make clean
make -j8
make install
echo "The Compilation of x264 for $ANDROID_ABI is completed"
}

build_x264


## armeabi-v7a
#ANDROID_ABI=armeabi-v7a
#ANDROID_API=android-14
#ANDROID_ARCH=arch-arm
#ANDROID_EABI=arm-linux-androideabi-4.9
#
#HOST=arm-linux-androideabi
#CROSS_COMPILE=arm-linux-androideabi-
#
#build_x264
#
## arm64-v8a
#ANDROID_ABI=arm64-v8a
#ANDROID_API=android-21
#ANDROID_ARCH=arch-arm64
#ANDROID_EABI=aarch64-linux-android-4.9
#
#HOST=aarch64-linux-android
#CROSS_COMPILE=aarch64-linux-android-
#
#build_x264