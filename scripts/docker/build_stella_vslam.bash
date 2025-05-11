#!/bin/bash

NUM_THREADS="$(expr $(nproc) - 1)"
# cd /stella_vslam && \
#   cd build && \
#   sudo cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo .. && \
#   sudo make -j4 && \
#   sudo make install

cd /stella_vslam && \
  cd build && \
  sudo cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_CXX_FLAGS="-fno-omit-frame-pointer -g2 -O0" \
      -DUSE_BACKTRACE=ON \
      -DENABLE_STACKTRACE=ON .. && \
  sudo make -j4 && \
  sudo make install

  cd /stella_vslam/stella_vslam_examples/build