This program uses SFML 3 as the UI code. To run this code, you will need a specific file called "arial.ttf". 
This file should live in the cmake-build-debug file. 
It might not be necessary, but when trying to build and run this code, this issue would always pop up. 


In the CMakeLists.txt file, make sure to add in the libraries for the SFML and load in SFML itself.

This is an example of a CMakeLists.txt file, that allows everything to run:

- The project name in this case is '3530Project3'

cmake_minimum_required(VERSION 3.31)
project(3530Project3)

set(CMAKE_CXX_STANDARD 14)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(3530Project3
src/DayList.cpp
src/DayList.h
src/HeapSort.cpp
src/HeapSort.h
src/main.cpp
src/PerlinNoise.cpp
src/PerlinNoise.h
src/QuickSort.cpp
src/QuickSort.h)

configure_file(
${CMAKE_SOURCE_DIR}/src/arial.ttf
${CMAKE_BINARY_DIR}/arial.ttf
COPYONLY
)

target_link_libraries(3530Project3
PRIVATE
SFML::Graphics
SFML::Window
SFML::System
)
