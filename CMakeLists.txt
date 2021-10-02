# Heavily inspired from https://github.com/Andrew9317/qt-cmake-template/blob/main/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME AudioPlayer)

if(WIN32)
    set(QT_HOME "C:/Qt/6.1.3/msvc2019_64")
else()
    set(QT_HOME "/home/frederic/Programs/Qt/6.1.0/gcc_64")
endif()

project(${TARGET_NAME} VERSION 0.0.1 DESCRIPTION "Project Description")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH ${QT_HOME})

find_package(Qt6 COMPONENTS Widgets REQUIRED PATHS ${QT_HOME})

add_executable(${TARGET_NAME}
    # Put all header containing objects using the slots/signals mechanism
    include/ui/qt_view.hpp
    include/ui/main_view.hpp
    include/ui/views/menu_view.hpp
    # Sources list
    src/sounds/backends/portaudio_backend.cpp
    src/sounds/maths/complex.cpp
    src/sounds/maths/fourier.cpp
    src/sounds/music/pitch.cpp 
    src/sounds/players/sound_files_player.cpp
    src/sounds/processors/amplifier.cpp
    src/sounds/processors/delay.cpp
    src/sounds/processors/echo.cpp
    src/sounds/processors/equalizer.cpp
    src/sounds/processors/fir_filter.cpp
    src/sounds/processors/varying_amplifier.cpp
    src/sounds/sounds/files/mp3_file.cpp
    src/sounds/sounds/files/wav_file.cpp
    src/sounds/sounds/synth/basic/fourier_wave.cpp
    src/sounds/sounds/synth/basic/sine_wave.cpp
    src/sounds/sound_file.cpp
    src/sounds/sound_player.cpp
    src/ui/views/player/effects/echo_view.cpp
    src/ui/views/player/effects/equalizer_view.cpp
    src/ui/views/player/visualizers/frequency_spectrum.cpp
    src/ui/views/player/visualizers/phases_spectrum.cpp
    src/ui/views/player/visualizers/signal_wave.cpp
    src/ui/views/player/controls_view.cpp
    src/ui/views/player/effects_view.cpp
    src/ui/views/player/visualizers_view.cpp
    src/ui/views/sidebar/file_view.cpp
    src/ui/views/sidebar/queue_view.cpp
    src/ui/views/menu_view.cpp
    src/ui/views/player_view.cpp
    src/ui/views/sidebar_view.cpp
    src/ui/main_view.cpp
    src/app.cpp
    src/main.cpp
)
    
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/portaudio)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/openmp3)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib)

if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${TARGET_NAME} PRIVATE -Wextra -pedantic -Werror -Wno-error=unused-value -Wno-error=unused-parameter)
endif()

target_include_directories(${TARGET_NAME} PRIVATE ${Qt6_INCLUDE_DIRS})
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_OUTPUT_NAME "${TARGET_NAME}d" RELEASE_OUTPUT_NAME ${TARGET_NAME})

# Libraries
target_link_libraries(${TARGET_NAME} PRIVATE Qt6::Widgets portaudio_static openmp3)

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