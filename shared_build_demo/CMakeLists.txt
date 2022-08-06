cmake_minimum_required(VERSION 3.21)
project(shared_build_demo)

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v -stdlib=libc++ -framework OpenGL -framework AppKit -framework Security -framework CoreFoundation -framework CoreVideo -framework CoreMedia -framework QuartzCore -framework CoreFoundation -framework VideoDecodeAcceleration -framework Cocoa -framework AudioToolbox -framework VideoToolbox -framework OpenCL -framework CoreServices")

#设置头文件路径
set(INC_DIR build_local_mac/include)
set(SDL_INC_DIR /usr/local/include/SDL2)

#设置链接库路径
set(LINK_DIR build_local_mac/lib)
set(LIB_LINK_DIR /usr/local/lib)

#引入头文件
include_directories(${INC_DIR})
include_directories(${SDL_INC_DIR})
#include_directories(/usr/local/opt/openssl/include)

#引入库文件
link_directories(${LINK_DIR})
link_directories(${LIB_LINK_DIR})
#link_directories(/usr/local/opt/openssl/lib)

add_executable(shared_build_demo main.cpp output_log.cpp)

#将第三方库连接在一起
target_link_libraries(shared_build_demo
        libavcodec.a libavdevice.a libavfilter.a libavformat.a libavutil.a libpostproc.a libswresample.a libswscale.a
        pthread
        libSDL2.a
        )
