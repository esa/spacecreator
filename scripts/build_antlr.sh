#!/bin/sh

# Package "uuid-dev" needs to be installed

echo "Building ANTLR C++ runtime"

cd 3rdparty/antlr
mkdir build
cd build
cmake -DCMAKE_BUILD_TIME=Release ../cpp_runtime
make -j8
cd ..
rm -rf build

echo "Result is in directory: 3rdparty/antlr/cpp_runtime/dist"
