#!/bin/bash -eu

# Use a static library as opposed to a dynamic library.
sed -i 's/SHARED/STATIC/g' ./src/CMakeLists.txt

mkdir build
cd build
cmake -DBUILD_TESTS=OFF -DBUILD_BENCHMARK=OFF ../
make

# Copy all fuzzer executables to $OUT/
$CXX $CXXFLAGS $LIB_FUZZING_ENGINE \
  $SRC/liburlparse/.clusterfuzzlite/url_fuzzer.cpp \
  -o $OUT/url_fuzzer \
  $SRC/liburlparse/build/lib/liburlparse.a \
  -I$SRC/liburlparse/include
