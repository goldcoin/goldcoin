# Goldcoin Build Notes - IMPORTANT!

## Directory Structure
- **Source root**: `/home/microguy/git/microguy/goldcoin/`
- **Windows build dir**: `/home/microguy/git/microguy/goldcoin/build-win64/`
- **Linux build dir**: `/home/microguy/git/microguy/goldcoin/build/`
- **Depends**: `/home/microguy/git/microguy/goldcoin/depends/`

## ⚠️ CRITICAL RULES
1. **NO BOOST** - Completely banned, using C++23 standard library
2. **NO CMAKE CHANGES** without discussion
3. **BUILD IN CORRECT DIRECTORY** - Not in root!

## Windows Qt Build (goldcoin-qt.exe)
```bash
# ALWAYS cd to build-win64 first!
cd /home/microguy/git/microguy/goldcoin/build-win64

# Configure for Windows cross-compile
cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/Win64Toolchain.cmake \
         -DCMAKE_PREFIX_PATH=/home/microguy/git/microguy/goldcoin/depends/x86_64-w64-mingw32 \
         -DBUILD_GUI=ON \
         -DBUILD_TESTS=OFF \
         -DBUILD_RUST=OFF

# Build (16 cores available!)
make goldcoin-qt -j16
```

## Current Status
- ✅ goldcoind.exe builds and works
- ✅ goldcoin-cli.exe builds and works  
- ❌ goldcoin-qt.exe builds but fails at 257KB due to Qt 6 refactoring needed
- ✅ Boost completely purged from system
- ✅ Using C++23 with GCC 15
- ✅ Qt 6.9.0 built in depends for Windows

## Qt 6 Location
- Windows Qt 6: `/home/microguy/git/microguy/goldcoin/depends/work/build/x86_64-w64-mingw32/qt6/6.9.0-c48cf0ef9dc/`
- Linux Qt 6: System Qt 6.4.2 (`/usr/lib/x86_64-linux-gnu/cmake/Qt6/`)

## Common Issues
1. **Build files in wrong directory**: Clean root with:
   ```bash
   cd /home/microguy/git/microguy/goldcoin
   rm -f CMakeCache.txt cmake_install.cmake Makefile
   rm -rf CMakeFiles/
   ```

2. **Boost contamination**: Run check script:
   ```bash
   ./check_boost_ban.sh
   ```

3. **Qt 6 not found**: Qt is built but may need proper installation to prefix

## What Works
- goldcoind.exe (daemon) - TESTED
- goldcoin-cli.exe (CLI) - TESTED
- txindex=1 for full validation

## What Needs Fixing
- goldcoin-qt.exe Qt 6 compilation errors
- Windows Qt 6 integration

## Pool Configuration (goldcoinpool.com)
```
rpcuser=goldcoinrpc
rpcpassword=<secure>
txindex=1
server=1
daemon=1
maxconnections=64
rpcbind=127.0.0.1
bind=127.0.0.1
```

## DO NOT FORGET
- We are on C++23, NOT C++17
- Qt 6.9, NOT Qt 5
- NO BOOST ANYWHERE
- BUILD IN build-win64/ FOR WINDOWS
- BUILD IN build/ FOR LINUX