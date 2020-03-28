cmake_minimum_required(VERSION 2.8)
project( video_stream )
set(raspicam_DIR "/usr/local/lib/cmake")
find_package(raspicam REQUIRED)
find_package( OpenCV REQUIRED )
add executable(video_stream video_stream.cpp)
target_link_libraries( video_stream ${OpenCV_LIBS} )
target_link_libraries( video_stream ${raspicam_CV_LIBS} )

