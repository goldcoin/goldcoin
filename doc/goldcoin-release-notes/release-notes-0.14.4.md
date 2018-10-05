Goldcoin Core version 0.14.4 is now available from:

  <https://www.goldcoin.org/downloads.html>

This is a new major version release, including new features, various bugfixes
and performance improvements, as well as updated translations.

Please report bugs using the issue tracker at github:

  <https://github.com/goldcoin/goldcoin/issues>

To receive security and update notifications, please subscribe to:

  <https://groups.google.com/forum/#!forum/goldcoin-dev>

Compatibility
==============

Goldcoin Core is extensively tested on multiple operating systems using
the Linux kernel, macOS 10.8+, and Windows Vista and later.

Microsoft ended support for Windows XP on [April 8th, 2014](https://www.microsoft.com/en-us/WindowsForBusiness/end-of-xp-support),
No attempt is made to prevent installing or running the software on Windows XP, you
can still do so at your own risk but be aware that there are known instabilities and issues.
Please do not report issues about Windows XP to the issue tracker.

Goldcoin Core should also work on most other Unix-like systems but is not
frequently tested on them.

Notable changes
===============

Fix crash bug with duplicate inputs within a transaction.  Original Bitcoin Fix:  bitcoin#14247

Credits
=======

Thanks to everyone who directly contributed to this release:

Stouse49 (2):
    Fix crash bug with duplicate inputs within a transaction
    Fix bitmap format of install wizard image

