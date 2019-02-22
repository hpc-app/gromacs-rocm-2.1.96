#!/bin/sh

/root/Desktop/cmake-3.13.0-rc2/bin/cmake -DBUILD_SHARED_LIBS=ON -DGMX_BUILD_FOR_COVERAGE=on -DCMAKE_BUILD_TYPE=Debug -DGMX_CUDA_NB_SINGLE_COMPILATION_UNIT=on -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DGMX_MPI=on -DGMX_GPU=on -DGMX_GPU_DETECTION_DONE=on -DHIP_TOOLKIT_ROOT_DIR=/opt/rocm/hip -DGMX_SIMD=AUTO -DCMAKE_INSTALL_PREFIX=/root/Desktop/gromacs-2018.4-install -DGMX_FFT_LIBRARY=fftw3 -DFFTWF_LIBRARY='/root/Desktop/fftw-3.3.8/build/lib/libfftw3f.so' -DFFTWF_INCLUDE_DIR='/root/Desktop/fftw-3.3.8/build/include'  -DBLAS_LIBRARIES='/root/Desktop/lapack-release-lapack-3.8.0/librefblas.so' -DLAPACK_LIBRARIES='/root/Desktop/lapack-release-lapack-3.8.0/liblapack.so' -DREGRESSIONTEST_DOWNLOAD=OFF -DCMAKE_PREFIX_PATH=/opt/rocm ..

