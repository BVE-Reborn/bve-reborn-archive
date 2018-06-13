#!/usr/bin/env bash

wget https://cmake.org/files/v3.10/cmake-3.10.1-Linux-x86_64.sh
mkdir -p $HOME/binary_root
bash cmake-3.10.1-Linux-x86_64.sh --skip-license --prefix=$HOME/binary_root --exclude-subdir
$HOME/binary_root/bin/cmake --version
bash ./build-deps-unix.sh

