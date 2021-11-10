# KchmViewer

This is a fork of the [kchmviewer](http://www.ulduzsoft.com/linux/kchmviewer) with some improvements.

- [Overview](#overview)
- [What's new](#whats-new)
- [Features](#features)
- [Getting KchmViewer](#getting-kchmviewer)
- [Usage](#usage)
- [Thanks](#thanks)
- [License](#license)


## Overview

KchmViewer is a chm (MS HTML help file format) and epub viewer, written in C++. Unlike most existing CHM viewers for Unix, it uses Trolltech Qt widget library, and does not depend on KDE or GNOME. It has full KDE 4 support.

The main advantage of KchmViewer is extended support for non-English languages. Unlike others, KchmViewer in most cases correctly detects chm file encoding, correctly shows tables of context of Russian, Korean, Chinese and Japanese help files. It also correctly searches text in non-English help files, including Korean, Chinese and Japanese.

KchmViewer is written by [Georgy Yunaev](https://github.com/gyunaev), and is licensed under GNU GPL license version 3.


## What's New

The 8.1 release is dedicated to the translation of the UI. Initially, translation worked only in an application which was built with KDE support. Now, this is possible with GNU Gettext for the target platform and is independent of KDE. However, the translation files are outdated and only Russian translation is up to date.
Read more in the [release notes](NEWS.md).


## Features

- Standalone viewer, depends on Qt 4 or Q t5 only. Does not require KDE, GNOME or wxWidgets toolkit.
- Could be optionally built with KDE 4 or KDE 5 support, using KDE dialogs.
- Completely safe and harmless. JavaScript can be disabled in the settings, optionally warns you before opening an external web page, or switching to another help file.
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


## Getting KchmViewer

The application is currently distributed in source code and you need to build it on your computer. This will not take long.

Go to the [build instructions page](INSTALL.md).


## Usage

Usage of KchmViewer is simple:

```
kchmviewer mychmfile.chm
```

for the rest of command-line options, see kchmviewer --help


## Thanks

Thanks to:

- [Jed Wing](https://github.com/jedwing), the author of [CHMLib](http://www.jedrea.com/chmlib/). This library is used by kchmviewer to access chm content.
- [Razvan Cojocaru](https://github.com/rzvncj), the author of [xCHM](https://xchm.sourceforge.io/). I used some ideas and chm processing code from xCHM.
- Peter Volkov for various bug reports and improvement suggestions.
- All the users, who report bugs, and suggest features. You help making kchmviewer better.

## License

KchmViewer is distributed under GNU GPL license version 3.
