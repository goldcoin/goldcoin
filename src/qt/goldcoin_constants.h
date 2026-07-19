// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

// bitcoin-config.h (and version_info.h) define PACKAGE_NAME as a preprocessor
// macro; undef it so it doesn't clobber the constexpr declaration below.
#ifdef PACKAGE_NAME
#undef PACKAGE_NAME
#endif

namespace goldcoin {
    inline constexpr auto PACKAGE_NAME = "Goldcoin Core";
    inline constexpr auto DEFAULT_SELFSIGNED_ROOTCERTS = false;
}

// For compatibility with existing code
using namespace goldcoin;