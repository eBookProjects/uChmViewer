# KchmViewer

- [Overview](#overview)
- [Features](#features)
- [Installations](#installations)
  - [Qt-only version](#qt-only-version)
  - [KDE4 version](#kde4-version)
- [Usage](#Usage)
- [Bug reporting](#bug-reporting)
- [Thanks](#thanks)
- [License](#license)


## Overview

KchmViewer is a chm (MS HTML help file format) viewer, written in C++. Unlike most existing CHM viewers for Unix, it uses Trolltech Qt widget library, and does not depend on KDE or GNOME. It has full KDE4 support.

The main advantage of KchmViewer is extended support for non-English languages. Unlike others, KchmViewer in most cases correctly detects chm file encoding, correctly shows tables of context of Russian, Korean, Chinese and Japanese help files. It also correctly searches text in non-English help files, including Korean, Chinese and Japanese.

KchmViewer is written by [Georgy Yunaev](mailto:gyunaev@ulduzsoft.com), and is licensed under GNU GPL license.


## Features

- Standalone viewer, depends on Qt4 or Qt5 only. Does not require KDE, GNOME or wxWidgets toolkit.
- Could be optionally built with KDE4 or KDE5 support, using KHTML and KDE dialogs.
- Completely safe and harmless. Does not support JavaScript in any way, optionally warns you before opening an external web page, or switching to another help file.
- Correctly detects and shows encoding of any valid chm file.
- Correctly shows non-English chm files, including Cyrillic, Chinese, Japanese and others.
- Correctly searches in non-English chm files using chm built-in search index.
- Shows an appropriate image for every TOC entry.
- Has complete chm index support, including multiple index entries, cross-links and parent/child entries in index.
- Persistent bookmarks support. Allows to store bookmarks even if "Favorites" window was not enabled for this chm file. Also stores the screen position for every bookmark. You can also edit/delete bookmarks.
- For any opened chm file, stores the last opened window, search history, bookmark history, font size and so on, so when you open this file again, everything is always on the place.
- Has easy and powerful search-in-page support.
- Allows to increase or decrease the font size, so physically handicapped people can read texts easily.
- Has standard Back/Forward/Home navigation.
- Can print the opened pages on a standard printer (usually via CUPS).
- Has complex search query support. You can use search queries like "lazy people" +learn -not.


## Installations

>Build with `Cmake` now supports both KDE and standalone versions. Refer to [BUILD.md](BUILD.md)</br>
>Also the `chmlib` library is included in the sources and therefore does not require an installed package with it.

Usually KchmViewer is distributed in source code archive, so you need to compile it first. It requires Qt version 4.4 or higher. Note that you need to install `qt4-devel` and `qt4-tools` packages (the last one might be included in `qt4-devel` in your distribution), not just qt package.

~~Also make sure you have `chmlib-devel` (some distros have it as `libchm-devel`) package installed. KDE build will check for its presence, but qmake does not have necessary functionality to do so. If you are getting errors regarding missing `chm_lib.h` file this means `chmlib-devel` is not installed.~~


### Qt-only version

To compile Qt-only version of KchmViewer, follow the procedure:

```
tar zxf kchmviewer-<version>.tar.gz
cd kchmviewer-<version>
qmake
make
```

The compiled binary is in bin/kchmviewer. You could copy it somewhere, or use it as-is. It does not require installation.

If `QtWebKit` module is not found, you will get the following error:

```
kchmviewwindow_qtwebkit.h:25:21: error: QWebView: No such file or directory
```

then you need to install the `QtWebKit` module.


### KDE4 version

To compile the version of KchmViewer with KDE4 support, follow the procedure:

```
tar zxf kchmviewer-<version>.tar.gz
mkdir build
cd build
cmake ..
make
sudo make install
```

For KDE version the installation is required, since the KHTML KIO slave cannot be used in place.


## Usage

Usage of KchmViewer is simple:

```
kchmviewer mychmfile.chm
```

for the rest of command-line options, see kchmviewer --help


## Bug reporting

Please use kchmviewer@ulduzsoft.com for bug reporting.


## Thanks

Thanks to:

- [Jed Wing](https://github.com/jedwing), the author of [chmlib](http://www.jedrea.com/chmlib/). This library is used by kchmviewer to access chm content.
- [Razvan Cojocaru](https://github.com/rzvncj), the author of [xCHM](https://xchm.sourceforge.io/). I used some ideas and chm processing code from xCHM.
- Peter Volkov for various bug reports and improvement suggestions.
- All the users, who report bugs, and suggest features. You help making kchmviewer better.

## License

KchmViewer is distributed under GNU GPL license version 3.
