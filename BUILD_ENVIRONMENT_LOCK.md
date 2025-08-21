# BUILD ENVIRONMENT LOCK - DO NOT MODIFY WITHOUT DISCUSSION
**Last Updated**: 2025-08-20 09:30
**Status**: LOCKED - These versions are FINAL

## Critical Dependencies - DO NOT CHANGE
| Component | Version | Hash/Details |
|-----------|---------|--------------|
| **GCC** | 15.2 | 15-20250816 snapshot |
| **Berkeley DB** | 18.1.40 | SHA256: 0cecb2ef0c67b166de93732769abdeba0555086d51de1090df325e18ee8da9c8 |
| **OpenSSL** | 1.0.1k | SHA256: 8f9faeaebad088e772f4ef5e38252d472be4d878c6b3a2718c10a4fcebe7a41c |
| **Qt** | 6.4.2 | System Qt (not from depends) |
| **CMake** | 3.28.3 | System version |
| **C++ Standard** | C++23 | NO BOOST - Pure stdlib only |

## Build Commands - USE THESE EXACTLY

### Linux Build (goldcoind daemon)
```bash
# Clean build directory
cd /home/microguy/git/microguy/goldcoin/build
rm -rf * 

# Configure with BDB 18.1.40 from depends
CC=gcc-15 CXX=g++-15 cmake .. \
  -DBUILD_GUI=OFF \
  -DBUILD_WALLET=ON \
  -DBUILD_TESTS=OFF \
  -DBUILD_RUST=OFF \
  -DCMAKE_PREFIX_PATH=/home/microguy/git/microguy/goldcoin/depends/x86_64-pc-linux-gnu \
  -DBDB_LIBRARY=/home/microguy/git/microguy/goldcoin/depends/x86_64-pc-linux-gnu/lib/libdb-18.1.a \
  -DBDB_CXX_LIBRARY=/home/microguy/git/microguy/goldcoin/depends/x86_64-pc-linux-gnu/lib/libdb_cxx-18.1.a \
  -DBDB_INCLUDE_DIR=/home/microguy/git/microguy/goldcoin/depends/x86_64-pc-linux-gnu/include

# Build
make goldcoind -j16
make goldcoin-cli -j16

# Verify static linking
ldd bin/goldcoind | grep db  # Should show NO db libraries
```

### Windows Cross-Compile
```bash
# First build depends (if needed)
cd depends
make HOST=x86_64-w64-mingw32 -j16

# Then build
cd ../build-windows
cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/Win64Toolchain.cmake \
         -DCMAKE_PREFIX_PATH=/home/microguy/git/microguy/goldcoin/depends/x86_64-w64-mingw32
cmake --build . -j16
```

## RULES
1. **NO DOWNGRADES** - We fix forward, not backward
2. **NO VERSION CHANGES** without team discussion
3. **BUILD ISSUES** - Fix the code, not the environment
4. **If it builds once**, it should build forever with these settings

## Current Build Status
- Linux goldcoind: ✅ WORKING (v0.17.0)
- Linux goldcoin-cli: ✅ WORKING (v0.17.0)
- Windows: 🚧 Needs testing with correct dependencies

## Known Working State
- Commit: Current feature/smart-pointer-migration branch
- Date: 2025-08-20
- goldcoind version: 0.17.0
- Checkpoint key: QfyEiNjN1BictFvjgPsLS2vDVTo3y969BAjdtbF1jjMAWA1tifbw

## Important Notes
- OpenSSL 1.0.1k is used for stability (OpenSSL 3.3.1 caused crashes on exit)
- BDB 18.1.40 must be statically linked from depends
- GCC 15 is required for C++23 support

---
⚠️ **IF YOU CHANGE THESE VERSIONS, DOCUMENT WHY HERE** ⚠️