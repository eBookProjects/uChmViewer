Developers tools
================


Astyle
------

https://astyle.sourceforge.net/astyle.html

Usage (from source root):

    astyle  --project=tools/astyle.cfg -I path/to/file.cpp

or

    astyle  --project=tools/astyle.cfg -I -r 'module/*.cpp' 'module/*.h'


Include-What-You-Use
--------------------

https://include-what-you-use.org
    
To use IWYU, run qmake with the optional USE flag:

    qmake  -r USE+=check_iwyu path/to/gcodeworkshop_top.pro

The following additional parameters are possible:

* `IWYU_INCLUDES` specifies additional paths to search for header files,
  this comes in handy in Debian:

      IWYU_INCLUDES=/usr/lib/llvm-13/include/c++/v1

* `IWYU_EXTRA_FLAGS` additional options specifically IWYU:

      IWYU_EXTRA_FLAGS='-Xiwyu --update_comments'


Additional links:

* https://github.com/include-what-you-use/include-what-you-use/blob/master/docs/IWYUMappings.md
* https://github.com/include-what-you-use/include-what-you-use/blob/master/docs/IWYUPragmas.md
