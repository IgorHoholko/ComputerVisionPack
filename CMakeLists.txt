cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

#cmake_policy(SET CMP0071 NEW)

project(ComputerVisionPack LANGUAGES C CXX)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Build OnnxRuntime
set(onnxruntime_BUILD_UNIT_TESTS OFF)
set(onnxruntime_BUILD_SHARED_LIB ON)
set(CMAKE_OSX_ARCHITECTURES arm64)
set(PYTHON_EXECUTABLE /usr/bin/python3)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd/onnxruntime/cmake onnxruntime)

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd/opencv4x)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(OpenCV 4.6.0 EXACT REQUIRED)



find_package(Qt6 REQUIRED COMPONENTS Core Widgets Core5Compat Multimedia)


#qt_standard_project_setup()


include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_INCLUDE_DIRS})


set(SOURCE
        src/tool/creative/cartonify.h
        src/tool/image/filter/fast_repeat_bilateral.h
        src/common.h
        src/tool/image/imgs_conversion.h
        src/tool/image/imgs_manipulation.h
        src/tool/image/smart_pad_resize.cpp
        src/gui/image_viewer.h
        src/gui/image_viewer.cpp
        src/gui/capture_thread.cpp
        src/gui/capture_thread.h
        src/gui/video_viewer.h
        src/gui/video_viewer.cpp
        src/gui/plugins/editor/editor_plugin_interface.h
        src/gui/edit_panel.h
        src/gui/edit_panel.cpp
        src/nn/onnx_runtime_wrapper.cpp
        src/nn/inference_wrapper.cpp
        src/nn/preprocess_image.h
        src/dl_models/base_model.h
        src/dl_models/detection/base_detector.cpp
        src/dl_models/detection/anchor_ssd.cpp
        src/dl_models/models_factory.cpp
        src/tool/detection/box_utils.h
        src/tool/detection/prior_box.h
        src/tool/detection/non_maximum_supression.cpp
        src/tool/geometry.h
        src/tool/image/visualization/color_map.cpp
        src/tool/image/visualization/boxes_drawer.cpp

        src/entities/box.cpp

        src/alg/detection_alg.cpp
        )

set(INCLUDE
        src/
        src/gui)

include_directories(${INCLUDE})
target_include_directories(onnxruntime INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/3rd/onnxruntime/include/onnxruntime/core/session ${CMAKE_CURRENT_SOURCE_DIR}/3rd/onnxruntime/include)


# PLUGINS
add_library(erode_plugin SHARED src/gui/plugins/editor/erode_plugin.cpp
        src/gui/edit_panel.cpp)
target_link_libraries(erode_plugin PRIVATE Qt6::Widgets ${OpenCV_LIBS})

add_library(cartonify_plugin SHARED src/gui/plugins/editor/cartonify_plugin.cpp
        src/gui/edit_panel.cpp)
target_link_libraries(cartonify_plugin PRIVATE Qt6::Widgets ${OpenCV_LIBS})

add_library(faces_detection_plugin SHARED src/gui/plugins/editor/faces_detection_plugin.cpp
        src/gui/edit_panel.cpp src/dl_models/models_factory.cpp ${SOURCE})
target_link_libraries(faces_detection_plugin PRIVATE Qt6::Widgets ${OpenCV_LIBS} Qt6::Core5Compat Qt6::Multimedia
        onnxruntime)
#FIXME: Split {SOURCE} to Qt coining and not



## EXECUTABLE
add_executable(main_image lib/main_image.cpp ${SOURCE})
target_link_libraries(main_image
        ${OpenCV_LIBS}
        Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Core5Compat Qt6::Multimedia
        onnxruntime
        )

add_executable(main_video lib/main_video.cpp ${SOURCE})
target_link_libraries(main_video
        ${OpenCV_LIBS}
        Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Core5Compat Qt6::Multimedia
        onnxruntime
        )


add_executable(faces_detect lib/faces_detect.cpp ${SOURCE})
target_link_libraries(faces_detect
        ${OpenCV_LIBS}
        Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Core5Compat Qt6::Multimedia
        onnxruntime
        )
