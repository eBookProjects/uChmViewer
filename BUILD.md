# Build KchmViewer

- [Dependencies and Tools](#dependencies-and-tools)
- [Cmake options](#cmake-options)
- [Debian](#debian)


## Dependencies and Tools

- C++11 compiler
- `cmake` or `qmake`
- `git`
- `libzip-dev`
- SDK for `Qt4` or `Qt5` or `KDE4` or `KDE5`

When building with Qt5, QtWebKit QtWebKitWidgets are required and this may be a problem for versions above 5.5.


## Cmake options

- `-DQT4_ONLY=ON` to build without KDE, only with Qt4
- `-DQT5_ONLY=ON` to build without KDE, only with Qt5
- `-DCMAKE_PREFIX_PATH=path_to_qt` is the path to the Qt development package if it is simply unpacked without installation. In my case, Qt version 5.5 is in /home/user/Qt and I use `-DCMAKE_PREFIX_PATH=~/Qt/5.5/gcc/`


## Debian

```sh
apt install build-essential cmake git libzip-dev
git clone https://github.com/gyunaev/kchmviewer
cd kchmviewer
mkdir build
cd build
# run cmake
cmake ../ && cmake --build . -j3
# or qmake
qmake ../ && make -j3
```
