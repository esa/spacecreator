#!/bin/sh

# Requirements
# Package "uuid-dev"
# Ninja build tool
# cmake

echo "Building ANTLR C++ runtime"

export CXX=$(which g++)
cd 3rdparty/antlr
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -G Ninja ../cpp_runtime
ninja antlr4_static
cd ..
rm -rf build

echo "Result is in directory: 3rdparty/antlr/cpp_runtime/dist"
