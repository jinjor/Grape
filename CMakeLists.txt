cmake_minimum_required(VERSION 3.15)

PROJECT(JUCE_CMAKE_EXAMPLE
LANGUAGES CXX
VERSION 0.0.1
)

add_subdirectory(libs/JUCE)
add_subdirectory(data)
add_subdirectory(src)
