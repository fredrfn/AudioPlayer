cmake_minimum_required(VERSION 3.16)
project(openmp3 VERSION 1.0.0 DESCRIPTION "OpenMP3 library custom CMakeLists build")
set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(openmp3 STATIC 
    src/decoder.cpp
    src/huffman.cpp 
    src/iterator.cpp 
    src/requantize.cpp
    src/stereo.cpp 
    src/synthesis.cpp 
    src/tables.cpp
)

install(TARGETS openmp3
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES openmp3.h DESTINATION include)