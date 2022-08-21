cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)

project(ComputerVisionPack LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(THIRD_PARTY_DIR 3rd/)


find_package(OpenCV 4.6.0 EXACT REQUIRED)


include_directories(${OpenCV_INCLUDE_DIRS}) # not needed for opencv>=4.0
link_directories(${OpenCV_INCLUDE_DIRS})


set(SOURCE
        src/utils/creative/cartonify.h
        src/tool/filter/fast_repeat_bilateral.h)

set(INCLUDE
        src/)

include_directories(${INCLUDE})


add_executable(color_balance_exec lib/exec/image_processing/color_balance_exec.cpp ${SOURCE})
target_link_libraries(color_balance_exec
        ${OpenCV_LIBS})


add_executable(cartonify_exec lib/exec/image_processing/cartonify_exec.cpp ${SOURCE} )
target_link_libraries(cartonify_exec
        ${OpenCV_LIBS})

