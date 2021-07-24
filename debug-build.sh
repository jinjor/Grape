#!/bin/bash

set -e

cmake \
  -B build \
  -G 'Unix Makefiles' \
  -D CMAKE_BUILD_TYPE=Debug \
  -D JUCE_COPY_PLUGIN_AFTER_BUILD=ON \
  -D JUCE_BUILD_EXTRAS=ON
  
cmake --build build