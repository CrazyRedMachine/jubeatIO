cmake_minimum_required(VERSION 3.14)
project(jubeatIO-device)

set(CMAKE_CXX_STANDARD 14)

include_directories(${jubeatIO-device_SOURCE_DIR})
link_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_library(device SHARED device.cpp device.h)
target_link_libraries(device hid)
SET_TARGET_PROPERTIES(device PROPERTIES PREFIX "")
