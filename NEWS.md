## 2022-02-12 v8.2-rc

The release adds support for KDE Framework 5 instead of KDE4 and initial support for Qt6 instead of Qt4.

* Fork renamed to uChmViewer. Also removed the code checking for updates.
* Ended support for Qt4 and KDE4 on the master branch. Qt4 specific code has been removed.
* Added support for the KDE Framework 5 with KDELibs4Support.
* Added limited support for Qt6. The application is built with Qt6.2, but had to disable printing and rely on default settings when browsing pages.
* Added USE_DBUS option to the CMake build script. The option allows you to enable/disable building with D-Bus on any platform where this technology is available. Previously, building with D-Bus was supported only on Linux.


## 2021-11-15 v8.1-rc2

* Fixed build failure with Qt WebKit. In some cases compilation of the src/qtwebkit/viewwindowmgr.cpp file failed with the message `error: invalid use of incomplete type 'class QTabBar'`.


## 2021-11-10 v8.1-rc

This release is dedicated to the translation of the UI. Initially, translation worked only in an application which was built with KDE support.

* Added KDE-independent UI translation support using GNU Gettext. QT and KDE dialogs and messages are also translated if appropriate files are available.
* Updated translation into Russian.
* The build script has been updated to better support application installation on Windows and macOS.
* Fixed a bug in showing pages of some EPUB files. EPUB files contain XML, but the app treated them as HTML. And if XML contains self-closing `title` tag, then browser was considered it as invalid HTML and did not show content.
* Fixed a bug in a file filter for the Open File dialog box in KDE. Due to an error in the filter description, the Open File dialog shows only CHM files. Now, the dialog has three display options:
  - All supported books
  - CHM only
  - EPUB only
* Fixed a bug of parsing command line arguments with non-Latin characters, occurring in KDE.


## 2021-09-20 v8.0-rc

This release has two goals: the option to use QtWebEngine and the migration to cmake as the primary build system.

* Full support for QtWebEngine since Qt 5.9, including the ability to change page encoding.
* When building an application, it is possible to choose HTML engine: QtWebKit or QtWebEngine.
* Updated Cmake scripts to build applications without depending on KDE. This allows Cmake to build applications for Windows, macOS and Linux using only Qt.
* Added initial support for Cpack.
