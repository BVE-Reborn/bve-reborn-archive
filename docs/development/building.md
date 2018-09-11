# Building {#building}

All platforms will need an install of boost 1.64+ and cmake 3.12+. Linux installs will also need various system libraries for the graphics library bsf. These instructions assume knowlage about how to use cmake on your platform.

# Windows {#windows}

Building on windows first requires installation of boost. If you already know how to add boost to a cmake project on windows, you may skip this section.

First download boost 1.64.0 from [here](https://bsfproxy.connorwfitzgerald.com/bver/boost_1_64_0.zip) into the folder dependencies/binary/downloads. Extract the result into dependencies/binary/src/boost/. This will create a subfolder called boost_1_64_0. Then run build-deps-win.bat. This will go through the process of building boost. Then use cmake in the command line or the cmake-gui to turn the project into a visual studio solution. I don't recommend using the cmake integration directly, as it can be finicky and it doesn't work well on this particular codebase.

# Mac {#mac}

The boost versions provided by homebrew or macports are new enough for this project, so manual compilation shouldn't be needed.

## Brew

```commandline
brew install boost
```

## Macports

```commandline
sudo ports install boost
```

## Manual Compilation

```commandline
bash ./build-deps-unix
```

## Build 

After you have installed or compiled boost, then you can follow these commands to build the project using cmake.

```commandline
mkdir build
cd build
cmake ..
make
```

# Linux {#linux}

There are two different methods to building the codebase on linux. If you have a distribution that gives a version of boost that is 1.64+ (Ubunutu 18.04+) you can just install it from the command line using apt. If you aren't, you need to compile boost yourself. There is a helper script in the root directory that will automagically build boost and put it into a place that cmake can find it.

## Ubuntu 18.04+

```commandline
sudo apt install libgl1-mesa-dev libglu1-mesa-dev mesa-common-dev libx11-dev libxcursor-dev libxrandr-dev libxi-dev uuid-dev libicu-dev
sudo apt install libboost-all-dev # Ubuntu 18.04+
```

## Others

```commandline
bash ./build-deps-unix.sh
```

## Build 

After you have installed or compiled boost, then you can follow these commands to build the project using cmake.

```commandline
mkdir build
cd build
cmake ..
make
```
