# Getting started

## Prerequisites
- [CMake](https://cmake.org/) (minimum required version: 3.5)
- [Qt](https://www.qt.io/) (tested with 5.15.2)

## Download project
```sh
git clone https://github.com/niklashempel/qt-beleg
cd qt-beleg
```


## Configure CMake
```sh
# Configure CMake in a 'build' subdirectory. `CMAKE_BUILD_TYPE` is only required if you are using a single-configuration generator such as make.
# See https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html?highlight=cmake_build_type
cmake -B ./build -DCMAKE_BUILD_TYPE=Release
```

## Build
```sh
cmake --build ./build --config Release
```

## Run
```sh
./build/qt-beleg
```
