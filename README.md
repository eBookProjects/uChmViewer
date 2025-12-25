uChmViewer
================================================================================

This is a fork of the [KchmViewer](http://www.ulduzsoft.com/linux/kchmviewer)
with some improvements. uChmViewer is a chm (MS HTML help file format) and epub
viewer, written in C++ using Qt framework.

- [Overview](#overview)
- [Features](#features)
- [Getting uChmViewer](#getting-uchmviewer)
- [Usage](#usage)
- [Thanks](#thanks)
- [License](#license)


Overview
--------------------------------------------------------------------------------

The main advantage of uChmViewer is extended support for non-English languages.
Unlike others, uChmViewer in most cases correctly detects chm file encoding,
correctly shows tables of context of Russian, Korean, Chinese and Japanese help
files. It also correctly searches text in non-English help files, including
Korean, Chinese and Japanese.

Original KchmViewer is written by [Georgy Yunaev](https://github.com/gyunaev).


Features
--------------------------------------------------------------------------------

- Standalone viewer, depends on Qt 5 or Qt 6 only. Does not require KDE, GNOME
  or wxWidgets toolkit.
- Completely safe and harmless. JavaScript can be disabled in the settings,
  optionally warns you before opening an external web page, or switching to
  another help file.
- Correctly detects and shows encoding of any valid chm file.
- Correctly shows non-English chm files, including Cyrillic, Chinese, Japanese
  and others.
- Correctly searches in non-English chm files using chm built-in search index.
- Shows an appropriate image for every TOC entry.
- Has complete chm index support, including multiple index entries, cross-links
  and parent/child entries in index.
- Persistent bookmarks support. Allows to store bookmarks even if "Favorites"
  window was not enabled for this chm file. Also stores the screen position for
  every bookmark. You can also edit/delete bookmarks.
- For any opened chm file, stores the last opened window, search history,
  bookmark history, font size and so on, so when you open this file again,
  everything is always on the place.
- Has easy and powerful search-in-page support.
- Allows to increase or decrease the font size, so physically handicapped people
  can read texts easily.
- Has standard Back/Forward/Home navigation.
- Can print the opened pages on a standard printer (usually via CUPS).
- Has complex search query support. You can use search queries like
`"lazy people" +learn -not`.


Getting uChmViewer
--------------------------------------------------------------------------------

Binary builds for Debian, Ubuntu, Fedora and Windows are available on the
[release page](https://github.com/eBookProjects/uChmViewer/releases/latest).You
can also build the application yourself by following the
[build instructions](INSTALL.md).

In addition, third-party builds are available:

- For
  [OpenSuse](https://build.opensuse.org/package/show/home:AndnoVember:LXQt:Qt6/uchmviewer),
  thanks to Андрей Алыпов (AndnoVember).
- For
  [Fedora and EPEL](https://copr.fedorainfracloud.org/coprs/nucleo/uchmviewer),
  thanks to nucleo.

[![Packaging status](https://repology.org/badge/vertical-allrepos/uchmviewer.svg)](https://repology.org/project/uchmviewer/versions)


Usage
--------------------------------------------------------------------------------

Usage of uChmViewer is simple:

    uchmviewer mychmfile.chm

For the rest of command-line options, see `uchmviewer --help`.


Thanks
--------------------------------------------------------------------------------

Thanks to:

- [Georgy Yunaev](https://github.com/gyunaev), the author of original KchmViewer.
- [Jed Wing](https://github.com/jedwing), the author of
  [CHMLib](http://www.jedrea.com/chmlib/). This library is used by uChmViewer
  to access chm content.
- [Razvan Cojocaru](https://github.com/rzvncj), the author of
  [xCHM](https://xchm.sourceforge.io/). Georgy Yunaev used some ideas and chm
  processing code from xCHM.
- Peter Volkov for various bug reports and improvement suggestions.
- All the users, who report bugs, and suggest features. You help making
  uchmviewer better.


License
--------------------------------------------------------------------------------

uChmViewer is distributed under GNU GPL license version 3. You can get the
source code at the following sites:

- [GitHub](https://github.com/eBookProjects/uChmViewer)
- [GitFlic](https://gitflic.ru/project/u-235/uchmviewer)
- [Codeberg](https://codeberg.org/u-235/uChmViewer)
