# Building {#dev-building}

All platforms will need an install of cmake 3.12+. Linux installs will also need various system libraries for the graphics library nova. These instructions assume knowledge about how to use cmake on your platform.

---

# Windows {#windows}

Use cmake in the command line or the cmake-gui to turn the project into a visual studio solution. I don't recommend using the cmake integration directly, as it can be finicky and it doesn't work well on this particular codebase.

---

# Mac {#mac}


## Build 

After you have installed or compiled boost, then you can follow these commands to build the project using cmake.

```commandline
mkdir build
cd build
cmake ..
make
```

---

# Linux {#linux}

## Ubuntu

```commandline
sudo apt install libgl1-mesa-dev libglu1-mesa-dev mesa-common-dev libx11-dev libxcursor-dev libxrandr-dev libxi-dev uuid-dev libicu-dev
```

## Build 

You can follow these commands to build the project using cmake.

```commandline
mkdir build
cd build
cmake ..
make
```
