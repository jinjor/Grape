add_executable(GrapeBenchmarks)

target_compile_features(GrapeBenchmarks PUBLIC cxx_std_17)

target_link_libraries(GrapeBenchmarks
  PUBLIC
    benchmark::benchmark
    gtest_main
)

target_sources(GrapeBenchmarks
  PRIVATE
    Benchmark.cpp
)