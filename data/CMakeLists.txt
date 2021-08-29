add_executable(wavetable-gen WavetableGen.cpp)
target_compile_features(wavetable-gen PUBLIC cxx_std_17)

add_custom_command(
    OUTPUT
        lookup
        parabola
        saw
        sine
        white
        pink
    COMMAND wavetable-gen
    DEPENDS wavetable-gen
    VERBATIM)

add_custom_target(wavetables
    DEPENDS
        lookup
        parabola
        saw
        sine
        white
        pink
    VERBATIM)

juce_add_binary_data(GrapePluginBinaries SOURCES 
    ${CMAKE_BUILD_DIR}/${CMAKE_CURRENT_BINARY_DIR}/lookup
    ${CMAKE_BUILD_DIR}/${CMAKE_CURRENT_BINARY_DIR}/parabola
    ${CMAKE_BUILD_DIR}/${CMAKE_CURRENT_BINARY_DIR}/saw
    ${CMAKE_BUILD_DIR}/${CMAKE_CURRENT_BINARY_DIR}/sine
    ${CMAKE_BUILD_DIR}/${CMAKE_CURRENT_BINARY_DIR}/white
    ${CMAKE_BUILD_DIR}/${CMAKE_CURRENT_BINARY_DIR}/pink
)
add_dependencies(GrapePluginBinaries wavetables)