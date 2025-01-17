sudo cmake -D CUDNN_LIBRARY=/user/local/lib64/libcudnn.so \
      -D CMAKE_C_COMPILER=/usr/bin/gcc-11 \
      -D CMAKE_CXX_COMPILER=/usr/bin/g++-11 \
      -D CUDNN_INCLUDE_DIR=/usr/local/cuda/include \
      -D CMAKE_BUILD_TYPE=Release \
      -D CMAKE_INSTALL_PREFIX=/usr/local \
      -D WITH_HIGHUI=ON \
      -D WITH_GSTREAM=ON \
      -D WITH_FFMPEG=ON \
      -D WITH_NVCUVID=ON \
      -D WITH_CUDA=ON \
      -D WITH_CUDNN=ON \
      -D OPENCV_DNN_CUDA=ON \
      -D CUDA_ARCH_BIN="8.6" \
      -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules \
      -D OPENCV_CMAKE_CUDA_DEBUG=1 \
      ..
