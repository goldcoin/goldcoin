Goldcoin Core integration/staging tree
=====================================

[![Build Status](https://travis-ci.org/goldcoin/goldcoin.svg?branch=goldcoin-master)](https://travis-ci.org/goldcoin/goldcoin)

https://www.goldcoinproject.org

What is Goldcoin?
----------------
Launched on BCT in 2013. 

Goldcoin is an open-source cash payment system that scales on-chain without the need for L2 solutions. It enables instant payments to 
anyone, anywhere in the world, and can been sent **with or without a transaction fee**.

Goldcoin uses peer-to-peer technology to operate with no central authority: managing transactions 
and issuing money are carried out collectively by the network. The network capacity is 1,120 Tx/s with 2-min 
confirmations and 32 MB blocks. The network is SegWit-free and 0-Conf is fully supported.

For more information, as well as an immediately useable, binary version of
the Goldcoin Core software, see [https://www.goldcoinproject.org](https://www.goldcoinproject.org).

License
-------

Goldcoin Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

Development Process
-------------------

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/goldcoin/goldcoin/tags) are created
regularly to indicate new official, stable release versions of Goldcoin Core.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md).

The #core-client channel on [Discord](https://discord.me/goldcoin)
should be used to discuss complicated or controversial changes before working
on a patch set.

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test on short notice. Please be patient and help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write [unit tests](src/test/README.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`. Further details on running
and extending unit tests can be found in [/src/test/README.md](/src/test/README.md).

There are also [regression and integration tests](/qa) of the RPC interface, written
in Python, that are run automatically on the build server.
These tests can be run (if the [test dependencies](/qa) are installed) with: `qa/pull-tester/rpc-tests.py`

The Travis CI system makes sure that every pull request is built for Windows, Linux, and OS X, and that unit/sanity tests are run automatically.

### Manual Quality Assurance (QA) Testing

Changes should be tested by somebody other than the developer who wrote the
code. This is especially important for large or high-risk changes. It is useful
to add a test plan to the pull request description if testing the changes is
not straightforward.
