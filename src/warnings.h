// Copyright (c) 2007-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Copyright (c) 2011-2017 The Litecoin Core developers
// Copyright (c) 2013-2017 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#ifndef BITCOIN_WARNINGS_H
#define BITCOIN_WARNINGS_H

#include <stdlib.h>
#include <string>

void SetMiscWarning(const std::string& strWarning);
void SetfLargeWorkForkFound(bool flag);
bool GetfLargeWorkForkFound();
void SetfLargeWorkInvalidChainFound(bool flag);
bool GetfLargeWorkInvalidChainFound();
std::string GetWarnings(const std::string& strFor);

static const bool DEFAULT_TESTSAFEMODE = false;

#endif //  BITCOIN_WARNINGS_H
