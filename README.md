```sh
cmake -S D:/workspace/qt-beleg -B D:/workspace/qt-beleg/build/debug "-DCMAKE_GENERATOR:STRING=Ninja" "-DCMAKE_BUILD_TYPE:STRING=Debug" "-DCMAKE_PROJECT_INCLUDE_BEFORE:FILEPATH=D:\workspace\qt-beleg\build\debug/.qtc/package-manager/auto-setup.cmake" "-DQT_QMAKE_EXECUTABLE:FILEPATH=D:/Qt/6.5.0/mingw_64/bin/qmake.exe" "-DCMAKE_PREFIX_PATH:PATH=D:/Qt/6.5.0/mingw_64" "-DCMAKE_C_COMPILER:FILEPATH=D:/Qt/Tools/mingw1120_64/bin/gcc.exe" "-DCMAKE_CXX_COMPILER:FILEPATH=D:/Qt/Tools/mingw1120_64/bin/g++.exe" "-DCMAKE_CXX_FLAGS_INIT:STRING=-DQT_QML_DEBUG"
```
