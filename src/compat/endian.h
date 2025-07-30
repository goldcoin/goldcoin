// Copyright (c) 2014-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_COMPAT_ENDIAN_H
#define BITCOIN_COMPAT_ENDIAN_H

#if defined(HAVE_CONFIG_H)
#include "config/bitcoin-config.h"
#endif

#include <stdint.h>

// NEW APPROACH: Use compiler builtins for maximum compatibility
// This avoids platform-specific headers and works everywhere

// Detect host endianness at compile time
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ || \
    defined(__BIG_ENDIAN__) || \
    (defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN) || \
    (defined(_BYTE_ORDER) && _BYTE_ORDER == _BIG_ENDIAN) || \
    (defined(BYTE_ORDER) && BYTE_ORDER == BIG_ENDIAN) || \
    defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || \
    defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
#define WORDS_BIGENDIAN 1
#else
// Assume little-endian (true for macOS x86/ARM, Linux x86/ARM, Windows)
#define WORDS_BIGENDIAN 0
#endif

// Use compiler builtins for byte swapping (GCC/Clang support)
#if defined(__GNUC__) || defined(__clang__)
#define BSWAP_16(x) __builtin_bswap16(x)
#define BSWAP_32(x) __builtin_bswap32(x)
#define BSWAP_64(x) __builtin_bswap64(x)
#else
// Manual fallback for other compilers (unlikely on our platforms)
#define BSWAP_16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))
#define BSWAP_32(x) ((((x) >> 24) & 0xff) | (((x) >> 8) & 0xff00) | \
                     (((x) << 8) & 0xff0000) | (((x) << 24) & 0xff000000))
#define BSWAP_64(x) ((((x) >> 56) & 0xffULL) | (((x) >> 40) & 0xff00ULL) | \
                     (((x) >> 24) & 0xff0000ULL) | (((x) >> 8) & 0xff000000ULL) | \
                     (((x) << 8) & 0xff00000000ULL) | (((x) << 24) & 0xff0000000000ULL) | \
                     (((x) << 40) & 0xff000000000000ULL) | (((x) << 56) & 0xff00000000000000ULL))
#endif

// Define the endian conversion functions based on host endianness
#if WORDS_BIGENDIAN
// Big-endian host
#define htobe16(x) (x)
#define htole16(x) BSWAP_16(x)
#define be16toh(x) (x)
#define le16toh(x) BSWAP_16(x)

#define htobe32(x) (x)
#define htole32(x) BSWAP_32(x)
#define be32toh(x) (x)
#define le32toh(x) BSWAP_32(x)

#define htobe64(x) (x)
#define htole64(x) BSWAP_64(x)
#define be64toh(x) (x)
#define le64toh(x) BSWAP_64(x)
#else
// Little-endian host (macOS, most Linux, Windows)
#define htobe16(x) BSWAP_16(x)
#define htole16(x) (x)
#define be16toh(x) BSWAP_16(x)
#define le16toh(x) (x)

#define htobe32(x) BSWAP_32(x)
#define htole32(x) (x)
#define be32toh(x) BSWAP_32(x)
#define le32toh(x) (x)

#define htobe64(x) BSWAP_64(x)
#define htole64(x) (x)
#define be64toh(x) BSWAP_64(x)
#define le64toh(x) (x)
#endif


#endif // BITCOIN_COMPAT_ENDIAN_H