cmake_minimum_required(VERSION 3.15)

PROJECT(GRAPE_PLUGIN
LANGUAGES CXX
VERSION 0.0.1
)

add_subdirectory(libs/JUCE)
add_subdirectory(data)
add_subdirectory(src)
