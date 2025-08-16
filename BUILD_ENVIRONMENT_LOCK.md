# BUILD ENVIRONMENT LOCK - DO NOT MODIFY WITHOUT DISCUSSION
**Last Updated**: 2025-08-16
**Status**: LOCKED - These versions are FINAL

## Critical Dependencies - DO NOT CHANGE
| Component | Version | Hash/Details |
|-----------|---------|--------------|
| **Berkeley DB** | 18.1.40 | SHA256: 0cecb2ef0c67b166de93732769abdeba0555086d51de1090df325e18ee8da9c8 |
| **Boost** | 1.89.0 | SHA256: 85a33fa22621b4f314f8e85e1a5e2a9363d22e4f4992925d4bb3bc631b5a0c7a |
| **OpenSSL** | 3.3.1 | SHA256: 777cd596284c883375a2a7a11bf5d2786fc5413255efab20c50d6ffe6d020b7e |
| **Qt** | 6.9.0 | Via depends system |
| **CMake** | 3.27.9 | Via native_cmake in depends |
| **C++ Standard** | C++20 | NO FALLBACK |

## Build Commands - USE THESE EXACTLY

### Linux Build
```bash
cd build
cmake .. -DCMAKE_PREFIX_PATH=/home/microguy/git/microguy/goldcoin/depends/x86_64-pc-linux-gnu
cmake --build . -j16
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
- Linux: ✅ WORKING
- Windows: 🚧 OpenSSL 3.3.1 building, Boost 1.89 ready

## Known Working State
- Commit: [TO BE FILLED WHEN WINDOWS BUILD WORKS]
- Date: [TO BE FILLED]

---
⚠️ **IF YOU CHANGE THESE VERSIONS, DOCUMENT WHY HERE** ⚠️