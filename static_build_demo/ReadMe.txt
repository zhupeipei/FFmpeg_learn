ffmpeg源码拉下来后执行build.sh脚本

export LDFLAGS="-L/usr/local/opt/openssl/lib"
export CPPFLAGS="-I/usr/local/opt/openssl/include"

./configure --prefix=/Users/xmly/audio_video/FFmpeg_compile_local_mac/build_local_mac

