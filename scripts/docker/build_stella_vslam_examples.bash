NUM_THREADS="$(expr $(nproc) - 1)"

cd /stella_vslam/stella_vslam_examples && \
  cd build && \
  sudo cmake \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} \
    -DUSE_STACK_TRACE_LOGGER=ON \
    .. && \
  sudo make -j$NUM_THREADS