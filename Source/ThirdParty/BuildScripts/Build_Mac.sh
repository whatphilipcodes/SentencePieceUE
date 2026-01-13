#!/bin/bash

# Get script location
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PLUGIN_ROOT="$SCRIPT_DIR/../../.."
SOURCE_DIR="$PLUGIN_ROOT/Source/ThirdParty/sentencepiece_src"
BUILD_DIR="$PLUGIN_ROOT/Intermediate/ThirdParty/Build_Mac"
INSTALL_DIR="$PLUGIN_ROOT/Source/ThirdParty/Lib/Mac"

# Clean
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
mkdir -p "$INSTALL_DIR"

cd "$BUILD_DIR"

# CMake
# CMAKE_OSX_ARCHITECTURES="x86_64;arm64" creates a Universal Binary (Fat library)
# This works on both Intel Macs and Apple Silicon
cmake "$SOURCE_DIR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" \
    -DCMAKE_CXX_FLAGS="-fPIC" \
    -DSPM_ENABLE_SHARED=OFF \
    -DSPM_USE_BUILTIN_PROTOBUF=ON \
    -DSPM_ENABLE_TCMALLOC=OFF

# Build
cmake --build . --config Release --parallel

# Copy
cp src/libsentencepiece.a "$INSTALL_DIR/"
cp src/libsentencepiece_train.a "$INSTALL_DIR/"

echo "Build Complete. Check $INSTALL_DIR"