add_executable(wavetable-gen WavetableGen.cpp)
target_compile_features(wavetable-gen PUBLIC cxx_std_17)

add_custom_command(
    OUTPUT
        lookup
        parabla
        saw
        sine
    COMMAND wavetable-gen
#    MAIN_DEPENDENCY wavetable-gen
    DEPENDS wavetable-gen
    VERBATIM)

add_custom_target(wavetables lookup parabola saw sine)

juce_add_binary_data(GrapePluginBinaries SOURCES 
    ${CMAKE_CURRENT_BINARY_DIR}/lookup
    ${CMAKE_CURRENT_BINARY_DIR}/parabola
    ${CMAKE_CURRENT_BINARY_DIR}/saw
    ${CMAKE_CURRENT_BINARY_DIR}/sine
)