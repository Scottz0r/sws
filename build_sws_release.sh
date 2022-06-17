#!/bin/bash

# Change current directory to script location.
cd "$(dirname "$0")"

# Clean existing build directory
BUILD_DIR=build_release
rm -rf $BUILD_DIR
mkdir $BUILD_DIR

cd $BUILD_DIR

cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
