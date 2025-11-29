Building uChmViewer
===================

The application is written in C++ using the Qt framework. Building is possible
using `qmake` or `cmake`. Additionally, scripts are provided for packaging
the application into DEB, RPM packages, or a Windows installer.

- [Dependencies and tools](#dependencies-and-tools)
- [Getting sources](#getting-sources)
- [Manual building](#manual-building)
- [Creating the packages and the installers](#creating-the-packages-and-the-installers)
- [CMake options](#cmake-options)
- [Qmake options](#qmake-options)
- [Known issues](#known-issues)


Dependencies and tools
--------------------------------------------------------------------------------


### Mandatory requirements

- C++11 compiler which is supported by the [Qt SDK].
- Qt tools such as `moc` and the resource packer.
- `CMake` or `Qmake` to generate the build scripts.
  >Note that if you're building with `Qmake`, you don't need to install
  >the `CMake` utils.
- SDK for `Qt5` or `Qt6`. Minimum supported version is 5.5.
  >Note that with `Qt6` you need an additional `Core5Compat` module.
- The [CHMLib] library for handling chm files. You can choose dynamic or static
  linking. In the first case, you need a library and headers for development.
  In the second case you need to download the repository submodule and specify
  `USE_STATIC_CHMLIB` flag when generating the build script.
- The [libzip] library and headers for software development. When building this
  library from source you will need the [zlib] library.

>Note that building using scripts in the `package` folder may require additional
>packages; see the "Creating the packages and the installers" section for
>details.


### Optional requirements

- I recommend using [Git] to retrieve the source code. Although you can download
  and unpack the archive with the sources, this option is not covered here.
  Also, Git is more convenient if you want to use a submodule with CHMLib.
- [GNU Gettext] is required to translate the user interface.


### Preparation in Debian deriatives

In GNU/Linux with package DEB-based manager you need the following packages and
their dependencies:

- `build-essential` `cmake` `git`
- One of the following packages, depending on the required Qt and HTML engine:
  * `libqt5webkit5-dev`
  * `qtwebengine5-dev`
  * `qt6-webengine-dev` and `qt6-5compat-dev`
- `libzip-dev` `libchm-dev`
- `gettext`

>Note that the Ubuntu 22.04 (Jammy) distro does not have the `qt6-5compat-dev`
>library and you need to use `libqt6core5compat6-dev` instead.

>Note that starting with Ubuntu 25.04 (Plucky Puffin) and Debian 13 (Trixie),
>the `libqt5webkit5-dev` package is no longer available.


Packages for __Qt5__ and __WebKit__:

    apt install build-essential cmake git gettext \
    libzip-dev libchm-dev \
    libqt5webkit5-dev

Packages for __Qt5__ and __WebEngine__:

    apt install build-essential cmake git gettext \
    libzip-dev libchm-dev \
    qtwebengine5-dev

Packages for __Qt6__:

    apt install build-essential cmake git gettext \
    libzip-dev libchm-dev \
    qt6-webengine-dev qt6-5compat-dev


### Preparation in RedHat deriatives

In RedHat-based distributions you need the following packages and their
dependencies:

- `coreutils` `gcc` `make` `cmake` `git`
- One of the following packages, depending on the required Qt and HTML engine:
  * `qt5-qtwebkit-devel`
  * `qt5-qtwebengine-devel`
  * `qt6-qtwebengine-devel` and `qt6-qt5compat-devel`
- `libzip-devel` `chmlib-devel`
- `gettext`


Packages for __Qt5__ and __WebKit__:

    dnf install coreutils gcc make cmake git gettext \
    libzip-devel chmlib-devel \
    qt5-qtwebkit-devel

Packages for __Qt5__ and __WebEngine__:

    dnf install coreutils gcc make cmake git gettext \
    libzip-devel chmlib-devel \
    qt5-qtwebengine-devel

Packages for __Qt6__:

    dnf install coreutils gcc make cmake git gettext \
    libzip-devel chmlib-devel \
    qt6-webengine-devel qt6-5compat-devel


### Preparation in  Windows with MSYS2

Install the following packages:

- `make`
- `<prefix>gcc` `<prefix>gettext-runtime` `<prefix>gettext-tools`
- `<prefix>qt5-base` `<prefix>qtwebkit`
- `<prefix>libzip`

Packages for __x86_64__:

    pacman -Syu
    pacman -S make mingw-w64-x86_64-gcc \
        mingw-w64-x86_64-gettext-runtime mingw-w64-x86_64-gettext-tools \
        mingw-w64-x86_64-qt5-base mingw-w64-x86_64-qtwebkit \
        mingw-w64-x86_64-libzip

Or using __pacboy__:

    pacman -Syu
    pacman -S pactoys
    pacboy make: gcc:p gettext-runtime:p gettext-tools:p \
        qt5-base:p qtwebkit:p libzip:p


Getting sources
--------------------------------------------------------------------------------

Use the `git clone` command to retrieve the sources. If you need static linking
to `CHMLib`, use `--recursive` options to download the `CHMLib` submodule.

    git clone --recursive https://github.com/eBookProjects/uChmViewer


Next, navigate to the folder source files, and create a `build` folder.

	mkdir uChmViewer/build
	cd uChmViewer/build

Now you're ready to build the application.


Manual building
--------------------------------------------------------------------------------

>Note that in Fedora, to select Qt5, you need to type `qmake-qt5` instead of
`qmake`.

Build with __Qt5__ and __WebKit__ using __CMake__

	cmake -DCMAKE_BUILD_TYPE=Release ../
	cmake --build . --config Release

Build with __Qt5__ and __WebKit__ using __Qmake__

	qmake -recursive CONFIG+=release USE_GETTEXT=1 ../
	make

Build with __Qt5__ and __WebEngine__ using __CMake__

	cmake -DCMAKE_BUILD_TYPE=Release -DUSE_WEBENGINE=ON ../
	cmake --build . --config Release

Build with __Qt5__ and __WebEngine__ using __Qmake__

	qmake -recursive CONFIG+=release USE_GETTEXT=1 USE_WEBENGINE=1 ../
	make

Build with __Qt6__ and __WebEngine__ using __CMake__

	cmake -DCMAKE_BUILD_TYPE=Release -DAPP_QT_VERSION=6 -DUSE_WEBENGINE=ON ../
	cmake --build . --config Release

Build with __Qt6__ and __WebEngine__ using __Qmake__

	qmake6 -recursive CONFIG+=release USE_GETTEXT=1 USE_WEBENGINE=1 ../
	make


Creating a packages and an installers
--------------------------------------------------------------------------------

The following scripts are provided for packaging the application:

- `package/build-deb.sh`
- `package/build-rpm.sh`
- `package/build-win-msys2.sh`

These scripts do not install any packages and do not require administrator
privileges. The scripts create a `build` folder in the current directory and
then build the application in it. After building, the package is copied to the
current directory. The scripts can accept options, a full description of which
is available with the `-h` switch. There are two commonly used options:

- `-q <qmake>` - specifies the name or full path of the `qmake` utility that
  the script should use.
- `-w <browser>` - specifies the HTML engine to use. Possible values:
  `WebEngine` or `WebKit` (default).


### Debian-based

Creating a package using the __Qt5__ and the __WebKit__

    package/build-deb.sh

Creating a package using the __Qt5__ and the __WebEngine__

    package/build-deb.sh -w webengine

Creating a package using the __Qt6__ and the __WebEngine__

    package/build-deb.sh -q qmake6 -w webengine


### RedHat-based

Install additional package.

    dnf install rpm-build

Creating a package using the __Qt5__ and the __WebKit__

    package/build-rpm.sh -q qmake-qt5

Creating a package using the __Qt5__ and the __WebEngine__

    package/build-rpm.sh -q qmake-qt5 -w webengine

Creating a package using the __Qt6__ and the __WebEngine__

    package/build-rpm.sh -q qmake6 -w webengine


### Windows using MSYS2

Install additional packages.

    pacboy zip: nsis:p ntldd:p

Creating an installer and archive with a portable version using the __Qt5__ and
the __WebKit__

    package/build-win-msys2.sh


### Using Cpack

CMake comes with a `cpack` utility for generating installers. The build scripts
have basic `cpack` support, but in some cases [additional options][Launch CPack]
and [variables][CPack module] may need to be specified.
The [generator][CPack generators] must be specified in any case.

Creating a deb package

	cmake -DCMAKE_BUILD_TYPE=Release ../
	cmake --build . --config Release
	cpack -G DEB -DCPACK_DEBIAN_PACKAGE_SHLIBDEPS=ON


CMake options
--------------------------------------------------------------------------------

* `-G` - specifies a build system generator. This option is not necessary when
  using the default toolkit.
* `--install-prefix <prefix>` - specifies a root folder for install application.
  By defaults to `/usr/local` on UNIX and `c:/Program Files/uchmviewer` on
  Windows. Note, that the installation prefix can be overridden when CMake is
  run in installation mode (`cmake --install`) with the `--prefix` option.
  Cpack ignores the installation prefix.
* `-D <var>=<value>` - specifies the initial value of the CMake script variable.

    **Project-specific variables**

    - `USE_STATIC_CHMLIB=<ON | OFF>` - choosing static linking with CHMLib,
      default is  `OFF`. For static linking, the library is compiled from source
      in the `lib/CHMLib` folder. This folder can be obtained along with the
      sources of the application using the command `git clone --recursive`.
    - `USE_WEBENGINE=<ON | OFF>` - choosing WebEngine instead of WebKit,
      default is `OFF`. WebEngine requires Qt version 5.9 or higher.
    - `USE_GETTEXT=<ON | OFF>` - enabling translation of the application,
      default is `ON`. If Gettext is not available, this option will be ignored.
    - `USE_DEPLOY_RUNTIME=<ON | OFF>` - copying runtime dependencies for
      deployment, default is `OFF`.
    - `USE_MACOS_BUNDLE=<ON | OFF>` - install as macOS bundle, default is `ON`.
    - `USE_MAC_APP=<ON | OFF>` - use derived QApplication, default to `ON` on
      macOS.
    - `USE_DBUS=<ON | OFF>` - use D-Bus integration, default to `ON` on Linux.
    - `chmlib_ROOT=<path/to/chmlib>`- specifies an additional folder to look up
      the library and headers for CHMLib. The `chm_lib.h` file is expected
      either in the root folder or the `include` folder. The `chm` library must
      be either in the root folder or in the `lib` folder.
    - `libzip_ROOT=<path/to/libzip>`- specifies an additional folder to look up
      the library and headers for libzip. The `zip.h` file is expected either in
      the root folder or the `include` folder. The `zip` library must be either
      in the root folder or in the `lib` folder.

    **CMake-specific variables**

    - `CMAKE_PREFIX_PATH=<path/to/qt>` is the path to the Qt development package
      if it is simply unpacked or builds without installation.
    - `CMAKE_BUILD_TYPE=<Debug | Release | RelWithDebInfo | MinSizeRel>`

  For example, configure cmake to build which Qt 5.15 and WebEngine

      cmake -DCMAKE_PREFIX_PATH=~/Qt/5.15/gcc -DUSE_WEBENGINE=ON ../


Qmake options
--------------------------------------------------------------------------------

The project options for Qmake are the similar to those for CMake.

- `USE_STATIC_CHMLIB=1` if defined, use static CHMLib from git submodule.
- `USE_WEBENGINE=1` if defined, use QtWebEngine.
- `USE_GETTEXT=1` if defined, the `msgfmt` command will be used to compile
  translations.
- `CHMLIB_ROOT=<path/to/chmlib>` folder to look up the library and headers for
  CHMLib.
- `LIBZIP_ROOT=<path/to/libzip>`folder to look up the library and headers for
  libzip.


Known issues
--------------------------------------------------------------------------------


### Peculiarities of Building in Ubuntu 18 (Bionic Beaver)

In addition to the listed above packages you should install qt5-default or
specify the full path `/usr/lib/i386-linux-gnu/qt5/bin/qmake` instead of `qmake`.

If the build crashes at the linking stage because the `GL` library is missing
even after installing the `libgl-dev` package, it may be needed to link
`/usr/lib/lib/libGL.so` to the regular library:

    ln /usr/lib/i386-linux-gnu/libGL.so.1.7.0 /usr/lib/libGL.so


### Building in Windows with MSYS2

Correct build is only possible under the following conditions:

- Using Qt5.
- Using QtWebKit.
- Using the built-in CHMLib library.


### Linker fails in macOS

In some cases, it is necessary to explicitly specify the architecture for which
the application is being build.

    cmake -DCMAKE_OSX_ARCHITECTURES=arm64 ../

    qmake -r QMAKE_APPLE_DEVICE_ARCHS=arm64 ../

See [Qt for macOS]


Useful links
--------------------------------------------------------------------------------

- Debian releases https://www.debian.org/releases/index
- Debian packages https://packages.debian.org
- Ubuntu releases https://documentation.ubuntu.com/project/release-team/list-of-releases
- Ubuntu packages https://packages.ubuntu.com
- Fedora packages https://packages.fedoraproject.org
- MSYS2 packages https://packages.msys2.org



[Qt SDK]: https://doc.qt.io/qt-5/supported-platforms.html "Qt5 doc: supported platforms"
[CHMLib]: https://github.com/jedwing/CHMLib "CHMLib repositoriy"
[libzip]: https://github.com/nih-at/libzip "libzip repositoriy"
[zlib]: https://github.com/madler/zlib "zlib repositoriy"
[Git]: https://git-scm.com/ "Git home page"
[GNU Gettext]: https://www.gnu.org/software/gettext "GNU Gettext home page"
[Launch CMake]: https://cmake.org/cmake/help/latest/manual/cmake.1.html "Launch CMake"
[Launch CPack]: https://cmake.org/cmake/help/latest/manual/cpack.1.html "Launch CPack"
[CPack module]: https://cmake.org/cmake/help/latest/module/CPack.html "CPack module"
[CPack generators]: https://cmake.org/cmake/help/latest/manual/cpack-generators.7.html "CPack generators"
[Qt for macOS]: https://doc.qt.io/qt-6/macos.html "Qt for macOS"
