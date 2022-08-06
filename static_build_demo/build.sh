export LDFLAGS="-L/usr/local/opt/openssl/lib"
export CPPFLAGS="-I/usr/local/opt/openssl/include"

./configure --prefix=/Users/xmly/audio_video/FFmpeg_compile_local_mac/build_local_mac

#./configure --prefix=/Users/xmly/audio_video/FFmpeg_compile_local_mac/build_local_mac \
#--enable-gpl \
#--enable-libx264
#--enable-openssl \
#--enable-version3 \
#--enable-nonfree \
#--enable-postproc \
#--enable-libass \
#--enable-libfreetype \
#--enable-libmp3lame \
#--enable-libopencore-amrnb \
#--enable-libopencore-amrwb \
#--enable-libopenjpeg \
#--enable-libopus \
#--enable-libspeex \
#--enable-libtheora \
#--enable-libvorbis \
#--enable-libvpx \
#--enable-libxvid
#--enable-libfdk-aac \
#--disable-static \
#--enable-shared
#--enable-libcelt \