#!/bin/bash

set -e

cmake \
  -B release-build \
  -G 'Unix Makefiles' \
  -D CMAKE_BUILD_TYPE=Release \
  -D JUCE_COPY_PLUGIN_AFTER_BUILD=ON
  
cmake --build release-build