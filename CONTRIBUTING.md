How to contribute to uChmViewer
===============================

Thank you for your interest in the project! Here are recommendations on
how to make a contribution. 


Have you found a bug?
---------------------

* Ensure the bug was not already reported in the
  [issues](https://github.com/u-235/uchmviewer/issues?q=type%3Aissue).
* If you have not found an open issue that solves the problem, open
  a [new one](https://github.com/u-235/uchmviewer/issues/new). Be sure
  to include a title and a clear description with information to help
  reproduce the bug. Useful information may be:

  - Operating system information
  - Information about the compiler, such as vendor and version.
  - Qt version
  - The application version or commit from which the application was build.
  - If the problem is specific to a certain file, attach it to the report.


Translations
------------

There is only one request for `.po` files: make sure the maximum line length
is 80 characters. This makes it easier to compare file versions. If your
translation tool does not support this option, you can call `msgmerge` in
`po` folder when you are done with the file:

    msgmerge -v -o uchmviewer_it.po uchmviewer_it.po uchmviewer.pot



Code style
----------

Follow these rules:

* Indent with tabs and alignment with spaces.
* Do not indent in the namespase block.
* Do not indent case statements in the switch block.
* Make sure that strings do not end with whitespace characters.
* Avoid multiple blank lines.

I use the following AStyle options:

    --indent=tab=4
    --indent-classes
    --indent-labels
    --indent-preproc-block
    --indent-preproc-define
    --convert-tabs
    --max-continuation-indent=64
    --keep-one-line-blocks
    --align-pointer=type
    --align-reference=type
    --pad-oper
    --pad-comma
    --pad-header
