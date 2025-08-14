# Windows Cross-Compilation Guide

## Overview
This guide documents the successful Windows cross-compilation of Goldcoin with ZERO external DLL dependencies.

## Prerequisites
```bash
sudo apt-get install mingw-w64 autoconf automake libtool pkg-config
```

## Build Instructions

### 1. Build Dependencies
```bash
# Use POSIX threading for proper C++11 support
export CC=x86_64-w64-mingw32-gcc-posix
export CXX=x86_64-w64-mingw32-g++-posix

# Build dependencies without Qt (for now)
make -C depends HOST=x86_64-w64-mingw32 NO_QT=1 -j$(nproc)
```

### 2. Configure
```bash
CONFIG_SITE=$PWD/depends/x86_64-w64-mingw32/share/config.site \
./configure --host=x86_64-w64-mingw32 \
    --prefix=/ \
    --disable-gui \
    --enable-wallet \
    --with-incompatible-bdb \
    --disable-tests \
    --disable-bench \
    CXXFLAGS="-std=c++17" \
    LDFLAGS="-static -static-libgcc -static-libstdc++"
```

### 3. Build
```bash
make -j$(nproc)
```

## Verification
```bash
# Check for external DLL dependencies
x86_64-w64-mingw32-objdump -p src/goldcoind.exe | grep "DLL Name"
```

Should only show Windows system DLLs:
- KERNEL32.dll
- WS2_32.dll
- ADVAPI32.dll
- etc.

## Known Issues & Solutions

### OpenSSL Version
- Use OpenSSL 1.1.1w instead of 3.x for compatibility
- Edit `depends/packages/openssl.mk` if needed

### Boost Libraries
- Specify exact library names: `--with-boost-system=boost_system-mt-s-x64`

### Threading Model
- Must use POSIX variant of MinGW compilers
- Standard MinGW lacks proper C++11 threading support

## Binary Sizes
- goldcoind.exe: ~18 MB
- goldcoin-cli.exe: ~7 MB
- goldcoin-tx.exe: ~7 MB

All sizes are for static builds with zero external dependencies.