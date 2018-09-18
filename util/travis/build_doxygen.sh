#!/bin/bash


pushd "$DEPS_DIR"

git clone https://github.com/doxygen/doxygen.git
mkdir build
cd build
cmake ../doxygen -DCMAKE_INSTALL_PREFIX=../doxygen-install
make install -j2

popd
