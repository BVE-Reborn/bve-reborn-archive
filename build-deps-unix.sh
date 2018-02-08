#!/bin/bash

#Set PWD to script location
cd "${0%/*}"

basedir=$PWD

if [ $# -ge 1 ]; then
	if [ "$1" == "--clean" ]; then
		rm -rf dependencies/binary/
		exit
	fi
	if [ "$1" == "--help" ]; then
		echo "Nothing fancy, pass --clean to clean all dependencies."
		exit
	fi
fi

# Get Newer CMake
mkdir -p dependencies/binary/{downloads,build,build,src/boost}
# wget -c -nc https://cmake.org/files/v3.10/cmake-3.10.2-Linux-x86_64.sh -O dependencies/binary/downloads/cmake-3.10.2-Linux-x86_64.sh
wget -c -nc https://dl.bintray.com/boostorg/release/1.64.0/source/boost_1_64_0.tar.bz2 -O dependencies/binary/downloads/boost_1_64_0.tar.bz2

# bash dependencies/binary/downloads/cmake-3.10.2-Linux-x86_64.sh --skip-license --prefix=$basedir/dependencies/binary/build --exclude-subdir

# Build Boost.... 
tar xjf dependencies/binary/downloads/boost_1_64_0.tar.bz2 -C dependencies/binary/src/boost
cd dependencies/binary/src/boost/boost_1_64_0/
./bootstrap.sh --prefix=$basedir/dependencies/binary/build --with-libraries=filesystem,system
./b2 link=static threading=multi variant=release 
./b2 install 
cd "${0%/*}"
