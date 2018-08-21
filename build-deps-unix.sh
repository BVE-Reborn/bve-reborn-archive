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

if [ "$(uname)" == "Darwin" ]; then 
	linking=static
else
	linking=shared
fi

# # Get Newer CMake
mkdir -p dependencies/binary/{downloads,build_x86,build_x64,src/boost}
wget -c -nc https://bsfproxy.connorwfitzgerald.com/bver/boost_1_64_0.tar.bz2 -O dependencies/binary/downloads/boost_1_64_0.tar.bz2

# # Build Boost.... 
tar xjf dependencies/binary/downloads/boost_1_64_0.tar.bz2 -C dependencies/binary/src/boost
cd dependencies/binary/src/boost/boost_1_64_0/
echo "using gcc : 7.1 : `type -p g++` ; " > tools/build/src/user-config.jam
./bootstrap.sh --prefix=$basedir/dependencies/binary/build_x64 --with-libraries=filesystem,system,regex
./b2 link=$linking threading=multi variant=release runtime-link=shared install -j$(($(nproc) + 1)) -d0
cd "${0%/*}"
