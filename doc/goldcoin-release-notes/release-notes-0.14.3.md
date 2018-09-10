Goldcoin Core version 0.14.3 is now available from:

  <https://www.goldcoin.org/get-started/>

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

Advanced Checkpointing when under a minimum hashrate

Checkpointing using the Feathercoin method was added to defend against 51% attacks.
Checkpoints are ignored if the hashrate is above a certain threshold, which increases over time.

Credits
=======

Thanks to everyone who directly contributed to this release:


Brett Walling (4):
      Removed unused paramater 'amount' due to SegWit removal to avoid warnings from gcc.
      Deleted verify_script.cpp, which contained duplicated functions, and removed it from the makefile.
      Added explicit braces to avoid ambiguous ‘else’
      Modified code to avoid comparison of signed and unsigned variables.

Stouse49 (11):
      51% Defense:  add missing time-too-old rule
      Windows Installer:  Replace obsolete GoldCoin Graphic with blank image
      Gitian:  fix repo for signatures
      Travis Tests (#3):  Use 1 thread to prevent timeouts
      GoldenRiver: Reduce logging (#18)
      Allow connections to GoldCoin 0.7.2 clients and older android apps with protocol 60018
      New Version:  0.14.3
      Update release notes for 0.14.2.1
      Update to new icons
      Add ACP (Advanced Checkpointing)
      Turn off synced checkpoint validation if hashrate is high enough
      Gitian:  Fix dir name

