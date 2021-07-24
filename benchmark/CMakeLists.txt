add_executable(GrapeBenchmarks)

target_compile_features(GrapeBenchmarks PUBLIC cxx_std_17)

# set_target_properties(GrapeBenchmarks
#   PROPERTIES
#     JUCE_GENERATED_SOURCES_DIRECTORY ${CMAKE_BUILD_DIR}/src/GrapePlugin_artefacts/JuceLibraryCode
# )

# juce_generate_juce_header(GrapeBenchmarks)

target_link_libraries(GrapeBenchmarks
  PUBLIC
    benchmark::benchmark
    gtest_main
)

target_sources(GrapeBenchmarks
  PRIVATE
    Benchmark.cpp
)