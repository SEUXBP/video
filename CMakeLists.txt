cmake_minimum_required(VERSION 3.4.1)

project(video)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-pthread -g")
#set(CMAKE_BUILD_TYPE "Release")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")




include_directories(include/)


# opencv
find_package(OpenCV REQUIRED)

set(CMAKE_INSTALL_RPATH "lib")

add_executable(video
    src/video.cc
)
target_link_libraries(video
  ${OpenCV_LIBS}
)


