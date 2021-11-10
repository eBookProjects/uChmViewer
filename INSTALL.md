# Building KchmViewer

- [Dependencies and Tools](#dependencies-and-tools)
- [Getting sources](#getting-sources)
- [Usage CMake](#usage-cmake)
- [Building in Debian](#building-in-debian)
- [Qmake options](#qmake-options)


## Dependencies and Tools


### Mandatory requirements

- C++11 compiler which is supported by the [Qt SDK][1].
- Qt tools such as `moc` and the resource packer.
- `CMake` or `Qmake` to generate the build scripts. For builds I recommend CMake, and Qmake is mainly used as a Qt Creator project.
- SDK for `Qt 4`, `Qt 5` or `KDE 4`.
- The [CHMLib][2] library for handling chm files. You can choose dynamic or static linking. In the first case, you need a library and headers for development. In the second case you need to download the repository submodule and specify `USE_STATIC_CHMLIB` flag when generating the build script.
- The [libzip][3] library and headers for software development. When building this library from source you will need the [zlib][4] library.


### Optional requirements

- I recommend using [Git][5] to retrieve the source code. Although you can download and unpack the archive with the sources, this option is not covered here. Also, Git is more convenient if you want to use a submodule with CHMLib.
- [GNU Gettext][6] is required to translate the user interface.


## Getting sources

Use the `git clone` command to retrieve the sources. The `-b release` option allows you to switch to the `release` branch immediately after downloading; this branch points to the latest release. If you need static linking to CHMLib, use `--recursive` options to download the CHMLib submodule.

```sh
# Getting the repository together with the submodule
# and switching to the release branch
git clone -b release --recursive https://github.com/u-235/kchmviewer
```


## Usage CMake

A modern [cmake][7] generates an build system script and also can be used as a build and installation driver.

```sh
# Generate build system
cmake [<options>] ../
# Bbilding mode
cmake --build . [<options>]
# Installation mode
cmake --install . [<options>]
```


### Options of build system generation

* `-G` - specifies a build system generator. This option is not necessary when using the default toolkit.
* `--install-prefix <prefix>` - specifies a root folder for install application. By defaults to `/usr/local` on UNIX and `c:/Program Files/kchmviewer` on Windows. Note, that the installation prefix can be overridden when CMake is run in installation mode (`cmake --install`) with the `--prefix` option. Cpack ignores the installation prefix.
* `-D <var>=<value>` - specifies the initial value of the CMake script variable.

    **Project-specific variables**

    - `FRAMEWORK=<Qt4 | Qt5 | KDE4>` - choosing the framework with which to build the application, default is `Qt5`.
    - `USE_STATIC_CHMLIB=<ON | OFF>` - choosing static linking with CHMLib, default is  `OFF`. For static linking, the library is compiled from source in the `lib/CHMLib` folder. This folder can be obtained along with the sources of the application using the command `git clone --recursive`.
    - `USE_WEBENGINE=<ON | OFF>` - choosing WebEngine instead of WebKit, default is `OFF`. WebEngine requires Qt version 5.9 or higher.
    - `USE_GETTEXT=<ON | OFF>` - enabling translation of the application, default is `ON`. If Gettext is not available, this option will be ignored.
    - `USE_DEPLOY_RUNTIME=<ON | OFF>` - copying runtime dependencies for deployment, default is `OFF`.
    - `chmlib_ROOT=<path/to/chmlib>`- specifies an additional folder to look up the library and headers for CHMLib. The `chm_lib.h` file is expected either in the root folder or the `include` folder. The `chm` library must be either in the root folder or in the `lib` folder.
    - `libzip_ROOT=<path/to/libzip>`- specifies an additional folder to look up the library and headers for libzip. The `zip.h` file is expected either in the root folder or the `include` folder. The `zip` library must be either in the root folder or in the `lib` folder.

    **CMake-specific variables**

    - `CMAKE_PREFIX_PATH=<path/to/qt>` is the path to the Qt development package if it is simply unpacked or builds without installation. In my case, Qt version 5.5 is in /home/user/Qt and I use `-DCMAKE_PREFIX_PATH=~/Qt/5.5/gcc`.
    - `CMAKE_BUILD_TYPE=<Debug | Release | RelWithDebInfo | MinSizeRel>`

  For example, configure cmake to build which Qt 5.15 and WebEngine

  ```sh
  cmake -DCMAKE_PREFIX_PATH=~/Qt/5.15/gcc -DUSE_WEBENGINE=ON ../
  ```


### Cpack

CMake comes with a `cpack` utility for generating installers. The build scripts have basic `cpack` support, but in some cases [additional options][8] and [variables][9] may need to be specified. The [generator][10] must be specified in any case.


## Building in Debian

In GNU/Linux with package manager DEB-based for any build configurations you need the following packages and their dependencies:

- `build-essential` `cmake` `git` `libzip-dev`
- `libchm-dev`
- `gettext`

The table lists the required additional packages and CMake options for varios  build configuration.

| Configuration    | Packages                                        | CMake Options        |
|------------------|-------------------------------------------------|----------------------|
| Qt 5 + WebKit    | `qt5-default` `libqt5webkit5-dev`               |                      |
| Qt 5 + WebEngine | `qt5-default` `qtwebengine5-dev-dev`            | `-DUSE_WEBENGINE=ON` |
| Qt 4             | `qt4-default` `libqtwebkit-dev`                 | `-DFRAMEWORK=QT4`    |
| KDE 4            | `qt4-default` `libqtwebkit-dev`  `kdelibs5-dev` | `-DFRAMEWORK=KDE4`   |


### Build with WebKit

```sh
apt install build-essential cmake git libzip-dev libchm-dev\
 gettext qt5-default libqt5webkit5-dev
git clone -b release --recursive https://github.com/u-235/kchmviewer
mkdir kchmviewer/build
cd kchmviewer/build
cmake -DCMAKE_BUILD_TYPE=Release ../
cmake --build . --config Release
# Optional generate package
cpack -G DEB -DCPACK_DEBIAN_PACKAGE_SHLIBDEPS=ON
```


### Build with WebEngine

```sh
apt install build-essential cmake git libzip-dev libchm-dev\
 gettext qt5-default qtwebengine5-dev
git clone -b release --recursive https://github.com/u-235/kchmviewer
mkdir kchmviewer/build
cd kchmviewer/build
cmake -DCMAKE_BUILD_TYPE=Release -DUSE_WEBENGINE=ON ../
cmake --build . --config Release
# Optional generate package
cpack -G DEB -DCPACK_DEBIAN_PACKAGE_SHLIBDEPS=ON
```


### Build with KDE4

```sh
apt install build-essential cmake git libzip-dev libchm-dev\
 gettext qt4-default libqtwebkit-dev kdelibs5-dev
git clone -b release --recursive https://github.com/u-235/kchmviewer
mkdir kchmviewer/build
cd kchmviewer/build
cmake -DCMAKE_BUILD_TYPE=Release -DFRAMEWORK=KDE4 ../
cmake --build . --config Release
# Optional generate package
cpack -G DEB -DCPACK_DEBIAN_PACKAGE_SHLIBDEPS=ON
```


## Qmake options

The project options for qmake are the similar to those for cmake.

- `USE_STATIC_CHMLIB=1` if defined, use static CHMLib from git submodule.
- `USE_WEBENGINE=1` if defined, use QtWebEngine.
- `CHMLIB_ROOT=<path/to/chmlib>` folder to look up the library and headers for CHMLib.
- `LIBZIP_ROOT=<path/to/libzip>`folder to look up the library and headers for libzip.


[1]: https://doc.qt.io/qt-5/supported-platforms.html "Qt doc: supported platforms"
[2]: https://github.com/jedwing/CHMLib "CHMLib repositoriy"
[3]: https://github.com/nih-at/libzip "libzip repositoriy"
[4]: https://github.com/madler/zlib "zlib repositoriy"
[5]: https://git-scm.com/ "Git home page"
[6]: https://www.gnu.org/software/gettext "GNU Gettext home page"
[7]: https://cmake.org/cmake/help/latest/manual/cmake.1.html "Launch cmake"
[8]: https://cmake.org/cmake/help/latest/manual/cpack.1.html "Launch cpack"
[9]: https://cmake.org/cmake/help/latest/module/CPack.html "Cpack module"
[10]: https://cmake.org/cmake/help/latest/manual/cpack-generators.7.html "cpack generators"
