cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)

project(ComputerVisionPack LANGUAGES C CXX)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(THIRD_PARTY_DIR 3rd/)

find_package(OpenCV 4.6.0 EXACT REQUIRED)

#set(Qt_DIR "/opt/homebrew/Cellar/qt/6.3.1_1/lib/cmake/Qt6")
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Core5Compat)
qt_standard_project_setup()


include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_INCLUDE_DIRS})


set(SOURCE
        src/utils/creative/cartonify.h
        src/tool/filter/fast_repeat_bilateral.h
        src/common/common.h
        src/common/imgs_conversion.h
        src/common/imgs_manipulation.h
        src/gui/image_viewer.h
        src/gui/image_viewer.cpp
        src/gui/plugins/editor/editor_plugin_interface.h
        src/gui/edit_panel.h
        src/gui/edit_panel.cpp
#        src/gui/plugins/editor/erode_plugin.h

        )

set(INCLUDE
        src/
        src/gui)

include_directories(${INCLUDE})


# PLUGINS
add_library(erode_plugin SHARED src/gui/plugins/editor/erode_plugin.cpp
        src/gui/edit_panel.cpp)
target_link_libraries(erode_plugin PRIVATE Qt6::Widgets ${OpenCV_LIBS})

add_library(cartonify_plugin SHARED src/gui/plugins/editor/cartonify_plugin.cpp
        src/gui/edit_panel.cpp)
target_link_libraries(cartonify_plugin PRIVATE Qt6::Widgets ${OpenCV_LIBS})

add_executable(main lib/main.cpp ${SOURCE})
target_link_libraries(main
        ${OpenCV_LIBS}
        Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Core5Compat)
#qt_import_plugins(main INCLUDE Qt::QLibInputPlugin)


#add_executable(color_balance_exec lib/exec/image_processing/color_balance_exec.cpp ${SOURCE})
#target_link_libraries(color_balance_exec
#        ${OpenCV_LIBS}
#        Qt6::Core Qt::Gui Qt6::Widgets)
#
#
#add_executable(cartonify_exec lib/exec/image_processing/cartonify_exec.cpp ${SOURCE} )
#target_link_libraries(cartonify_exec
#        ${OpenCV_LIBS})
#