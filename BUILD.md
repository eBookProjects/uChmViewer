# Building KchmViewer

- [Dependencies and Tools](#dependencies-and-tools)
- [CMake options](#cmake-options)
- [Qmake options](#qmake-options)
- [Building in Debian](#building-in-debian)


## Dependencies and Tools

- C++11 compiler which is supported by the [Qt SDK][1].
- Qt tools such as `moc` and the resource packer.
- `CMake` or `Qmake` to generate the build scripts. For builds I recommend CMake, and Qmake is mainly used as a QtCreator project.
- SDK for `Qt4`, `Qt5` or `KDE4`.
- The [CHMLib][2] library and headers for developing applications.
- The [libzip][3] library and headers for software development. When building this library from source you will need the [zlib][4] library.
- [Git][5]


## CMake options

- `FRAMEWORK=<Qt4 | Qt5 | KDE4>`, default is `Qt5` 
- `USE_STATIC_CHMLIB=<ON | OFF>`, default is  `OFF`
- `USE_WEBENGINE=<ON | OFF>`, default is `OFF`
- `chmlib_ROOT=<path_to_chmlib>`
- `libzip_ROOT=<path_to_libzip>`
- `CMAKE_PREFIX_PATH=<path_to_qt>` is the path to the Qt development package if it is simply unpacked or builds without installation. In my case, Qt version 5.5 is in /home/user/Qt and I use `-DCMAKE_PREFIX_PATH=~/Qt/5.5/gcc`

For example, configure cmake to build which Qt 5.15 and WebEngine

```sh
cmake -DCMAKE_PREFIX_PATH=~/Qt/5.15/gcc -DUSE_WEBENGINE=ON ../
```


## Qmake options

- `USE_STATIC_CHMLIB=1` if defined, use static chmlib from git submodule.
- `USE_WEBENGINE=1` if defined, use QtWebEngine.
- `CHMLIB_ROOT=<path_to_chmlib>`
- `LIBZIP_ROOT=<path_to_libzip>`


## Building in Debian

In deb-based GNU/Linux you need the following packages and their dependencies:

- `build-essential` `cmake` `git`
- `libzip-dev` `libchm-dev`
- `qt5-default`
- `libqt5webkit5-dev` or `qtwebengine5-dev`


### Build with WebKit

```sh
apt install build-essential cmake git libzip-dev libchm-dev qt5-default libqt5webkit5-dev
git clone --recursive https://github.com/u-235/kchmviewer
cd kchmviewer
git checkout v8.0
mkdir build
cd build
cmake ../ && cmake --build .
```


### Build with WebEngine

```sh
apt install build-essential cmake git libzip-dev libchm-dev qt5-default qtwebengine5-dev
git clone --recursive https://github.com/u-235/kchmviewer
cd kchmviewer
git checkout v8.0
mkdir build
cd build
cmake -DUSE_WEBENGINE=ON ../ && cmake --build .
```

[1]: https://doc.qt.io/qt-5/supported-platforms.html
[2]: https://github.com/jedwing/CHMLib
[3]: https://github.com/nih-at/libzip
[4]: https://github.com/madler/zlib
[5]: https://git-scm.com/ "Home page"
