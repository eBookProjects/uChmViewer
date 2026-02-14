Developers tools
================


Astyle
------

https://astyle.sourceforge.net/astyle.html

Usage (from source root):

    astyle  --options=tools/astyle.cfg -I path/to/file.cpp

or

    astyle  --options=tools/astyle.cfg -I -r 'module/*.cpp' 'module/*.h'


codespell
---------

https://github.com/codespell-project/codespell

Usage (from source root):

    codespell --config tools/codespell.cfg


Include-What-You-Use
--------------------

https://include-what-you-use.org

To use IWYU, run qmake with the optional USE flag:

    qmake  -r CHECK_IWYU=1 path/to/uchmviewer.pro

The following additional parameters are possible:

* `IWYU_INCLUDES` specifies additional paths to search for header files,
  this comes in handy in Debian:

      IWYU_INCLUDES=/usr/lib/llvm-13/include/c++/v1

* `IWYU_EXTRA_FLAGS` additional options specifically IWYU:

      IWYU_EXTRA_FLAGS='-Xiwyu --update_comments'

For a more complete check, use the `USE_GETTEXT` and `USE_MAC_APP` flags.

__Ignore message "should add these lines"__

    #include <QObject>  // for QTypeInfo<>::isComplex, QTypeInfo<>::isLarge, QTypeInfo<>::isRelocatable, QTypeInfo<>::isStatic
    #include <utility>  // for move


### Examples

For QtWebEngine:

    qmake -r CHECK_IWYU=1 USE_GETTEXT=1 USE_MAC_APP=1 USE_WEBENGINE=1 IWYU_EXTRA_FLAGS='-Xiwyu --update_comments' ..

For QtWebKit:

    qmake -r CHECK_IWYU=1 USE_GETTEXT=1 USE_MAC_APP=1 IWYU_EXTRA_FLAGS='-Xiwyu --update_comments' ..


Additional links:

* https://github.com/include-what-you-use/include-what-you-use/blob/master/docs/IWYUMappings.md
* https://github.com/include-what-you-use/include-what-you-use/blob/master/docs/IWYUPragmas.md
