NUM_THREADS="$(expr $(nproc) - 1)"

cd /stella_vslam && set -x && \
  cd build && \
  sudo cmake \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    .. && \
  sudo make -j$NUM_THREADS && \
  sudo make install
  