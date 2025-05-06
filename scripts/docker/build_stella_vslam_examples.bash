#!/bin/bash

NUM_THREADS="$(expr $(nproc) - 1)"

# cd /stella_vslam/stella_vslam_examples && \
#   cd build && \
#   sudo cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DUSE_STACK_TRACE_LOGGER=ON .. && \
#   sudo make -j4

cd /stella_vslam/stella_vslam_examples && \
  cd build && \
  sudo cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_CXX_FLAGS="-fno-omit-frame-pointer -g2 -O0" \
      -DUSE_BACKTRACE=ON \
      -DENABLE_STACKTRACE=ON .. && \
  sudo make -j4
  