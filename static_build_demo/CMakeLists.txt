cmake_minimum_required(VERSION 3.21)
project(FFmpeg_Clion_Test)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v -stdlib=libc++ -framework OpenGL -framework AppKit -framework Security -framework CoreFoundation -framework CoreVideo -framework CoreMedia -framework QuartzCore -framework CoreFoundation -framework VideoDecodeAcceleration -framework Cocoa -framework AudioToolbox -framework VideoToolbox -framework OpenCL -framework CoreServices ")

#设置头文件路径
#set(INC_DIR ffmpeg_lib/include)
set(INC_DIR build_local_mac/include)
set(SDL_INC_DIR /usr/local/include/SDL2)

#设置链接库路径
#set(LINK_DIR ffmpeg_lib/lib)
set(LINK_DIR build_local_mac/lib)
set(LIB_LINK_DIR /usr/local/lib)

#引入头文件
include_directories(${INC_DIR})
include_directories(${SDL_INC_DIR})
include_directories(/usr/local/Cellar/libiconv/1.16/include)
#include_directories(/usr/local/opt/openssl/include)

#引入库文件
link_directories(${LINK_DIR})
link_directories(${LIB_LINK_DIR})
#link_directories(/usr/local/opt/openssl/lib)
#link_directories(/usr/local/Cellar/x264/r3060/lib)
#link_directories(/usr/local/Cellar/libvpx/1.11.0/lib)
#link_directories(/usr/local/Cellar/libvorbis/1.3.7/lib)
#link_directories(/usr/local/Cellar/libvorbis/1.3.7/lib)
#link_directories(/usr/local/Cellar/aom/3.2.0_2/lib)
#link_directories(/usr/local/Cellar/openjpeg/2.4.0/lib)
#link_directories(/usr/local/Cellar/opus/1.3.1/lib)
#link_directories(/usr/local/Cellar/rav1e/0.5.0/lib)
#link_directories(/usr/local/Cellar/speex/1.2.0/lib)
#link_directories(/usr/local/Cellar/opencore-amr/0.1.5/lib/)
link_directories(/usr/local/Cellar/bzip2/1.0.8/lib)
link_directories(/usr/local/Cellar/libiconv/1.16/lib)
#link_directories(/usr/local/Cellar/xz/5.2.5/lib)
#link_directories(/Library/Developer/CommandLineTools/SDKs/MacOSX10.15.sdk/usr/lib/)

add_executable(FFmpeg_Clion_Test demo.cpp output_log.cpp)

#将第三方库连接在一起
target_link_libraries(FFmpeg_Clion_Test
#        iconv
        iconv.2.4.0
        charset
        libavfilter.a libavformat.a libavcodec.a libavutil.a libswresample.a libswscale.a libavdevice.a libpostproc.a
#        libavcodec.a libavdevice.a libavfilter.a libavformat.a libavutil.a libpostproc.a libswresample.a libswscale.a
        pthread
        libSDL2.a
        z
        bz2
        lzma
        )
