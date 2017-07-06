cmake_minimum_required(VERSION 3.3)
project(FlowSize)

set(PROJECT_BINARY_DIR "${PROJECT_SOURCE_DIR}/build/")
set(EXECUTABLE_OUTPUT_PATH  "${PROJECT_SOURCE_DIR}/bin")

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        CMSACSketch.h
        CMSketch.h
        common.h
        CUSACSketch.h
        CUSketch.h
        PSACSketch.h
        PSketch.h
        Sketch.h)

add_executable(FlowSizeIP ${SOURCE_FILES} main.cpp)
add_executable(FlowSizeSYN ${SOURCE_FILES} main_raw_str.cpp)
add_executable(FlowSizeGetCounter ${SOURCE_FILES} get_counter.cpp)