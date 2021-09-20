# Heavily inspired from https://github.com/Andrew9317/qt-cmake-template/blob/main/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME AudioPlayer)
set(QT_MAJOR_VERSION 6)
set(QT_HOME "C:/Qt/6.1.3/msvc2019_64")

project(${TARGET_NAME} VERSION 0.0.1 DESCRIPTION "Project Description")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH ${QT_HOME})

find_package(Qt6 COMPONENTS Widgets REQUIRED PATHS ${QT_HOME})

add_executable(${TARGET_NAME}
    include/ui/qt_view.hpp
    include/ui/main_window.hpp
    include/ui/views/menu_view.hpp
    src/audio/files/wav_file.cpp
    src/audio/audio_file.cpp
    src/audio/audio_player.cpp
    src/audio/backend/portaudio_backend.cpp
    src/ui/views/menu_view.cpp
    src/ui/views/player_view.cpp
    src/ui/views/player/controls_view.cpp
    src/ui/views/player/queue_view.cpp
    src/ui/main_window.cpp
    src/app.cpp
    src/main.cpp
)
    
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/portaudio)

if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

target_include_directories(${TARGET_NAME} PRIVATE ${Qt6_INCLUDE_DIRS})
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_OUTPUT_NAME "${TARGET_NAME}d" RELEASE_OUTPUT_NAME ${TARGET_NAME})

# Libraries
target_link_libraries(${TARGET_NAME} PRIVATE Qt6::Widgets portaudio_static)

# DLL copying
add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt6::Core>
        $<TARGET_FILE:Qt6::Gui>
        $<TARGET_FILE:Qt6::Widgets>
        $<TARGET_FILE_DIR:${TARGET_NAME}>
)
add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${Qt6_DIR}/../../../plugins/platforms
        $<TARGET_FILE_DIR:${TARGET_NAME}>/plugins/platforms
)

# Assets management
add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${TARGET_NAME}>/assets
)