# 🔒 Goldcoin Locked Build Environment
**Last Updated:** 2024-08-19  
**Status:** PRODUCTION READY

## Overview
This document defines the exact build environment for Goldcoin to ensure reproducible builds across all development machines and CI/CD pipelines.

## Core Compiler: GCC 15.2
**Version:** GCC 15-20250816 (Latest stable snapshot)  
**Standard:** C++23 with full feature support  
**Installation:** Built from source for maximum optimization

### Installation Steps
```bash
# 1. Prerequisites
sudo apt update && sudo apt install -y build-essential wget bzip2 flex libgmp-dev libmpfr-dev libmpc-dev

# 2. Download GCC 15.2
mkdir -p ~/gcc-build && cd ~/gcc-build
wget https://gcc.gnu.org/pub/gcc/snapshots/15-20250816/gcc-15-20250816.tar.xz

# 3. Extract and prepare
tar -xf gcc-15-20250816.tar.xz
cd gcc-15-20250816
./contrib/download_prerequisites

# 4. Configure build
mkdir ../gcc-15-build && cd ../gcc-15-build
../gcc-15-20250816/configure \
  --prefix=/usr/local/gcc-15 \
  --enable-languages=c,c++ \
  --disable-multilib \
  --program-suffix=-15 \
  --enable-checking=release \
  --with-system-zlib

# 5. Build with 16 cores (10-20 minutes on 16-core server)
make -j16

# 6. Install
sudo make install

# 7. Create system symlinks
sudo ln -sf /usr/local/gcc-15/bin/gcc-15 /usr/bin/gcc-15
sudo ln -sf /usr/local/gcc-15/bin/g++-15 /usr/bin/g++-15

# 8. Verify
gcc-15 --version
g++-15 --version
```

## Rust Toolchain
**Version:** 1.89.0  
**Edition:** 2021  
**Components:** rustc, cargo, rust-std

### Installation
```bash
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
rustup default stable
rustup update
```

## Build System
**CMake:** 3.28.3 or later  
**Make:** GNU Make 4.3+  
**Ninja:** 1.11+ (optional, for faster builds)

## Required Libraries

### System Libraries (via apt)
```bash
sudo apt install -y \
  libssl-dev \
  libevent-dev \
  libdb-dev \
  libdb++-dev \
  libsqlite3-dev \
  libminiupnpc-dev \
  libzmq3-dev \
  libprotobuf-dev \
  protobuf-compiler \
  libqrencode-dev
```

### Qt Framework
**Version:** 6.9.0  
**Modules:** Core, Gui, Widgets, Network, Test  
**Installation:** Built from source or via depends/

## C++23 Features Used
- Concepts and constraints
- Coroutines
- Ranges and views  
- std::expected
- std::format
- std::variant (replaced boost::variant)
- std::filesystem (replaced boost::filesystem)
- [[nodiscard]] attributes
- Structured bindings
- constexpr improvements

## Rust Features Used
- async/await with Tokio 1.40
- FFI with bindgen
- SIMD optimizations
- Parallel processing with Rayon
- Zero-copy serialization

## Build Flags

### Debug Build
```cmake
-DCMAKE_BUILD_TYPE=Debug
-DCMAKE_C_COMPILER=/usr/bin/gcc-15
-DCMAKE_CXX_COMPILER=/usr/bin/g++-15
-DBUILD_RUST=ON
-DBUILD_GUI=ON
-DBUILD_TESTS=ON
```

### Release Build
```cmake
-DCMAKE_BUILD_TYPE=Release
-DCMAKE_C_COMPILER=/usr/bin/gcc-15
-DCMAKE_CXX_COMPILER=/usr/bin/g++-15
-DBUILD_RUST=ON
-DBUILD_GUI=ON
-DBUILD_TESTS=OFF
-DCMAKE_CXX_FLAGS="-O3 -march=native -flto"
```

## Performance Optimizations
- Link-time optimization (LTO) enabled
- Profile-guided optimization (PGO) ready
- Native CPU architecture targeting
- Static linking for distribution builds

## Docker Build Environment
```dockerfile
FROM ubuntu:24.04
# Install GCC 15.2 as above
# Install Rust 1.89.0
# Configure for reproducible builds
```

## CI/CD Requirements
- GitHub Actions: ubuntu-24.04 runner
- Local builds: Ubuntu 24.04 LTS
- Cross-compilation: MinGW-w64 for Windows

## Version Lock File
```yaml
compiler:
  gcc: 15-20250816
  rust: 1.89.0
  
libraries:
  qt: 6.9.0
  openssl: 3.3.1
  boost: REMOVED (replaced with std)
  libevent: 2.1.12
  protobuf: 3.21.12
  
tools:
  cmake: 3.28.3
  make: 4.3
  cargo: 1.89.0
```

## Notes
- **NO BOOST**: All boost dependencies have been eliminated in favor of C++23 standard library
- **GCC 15 Required**: Earlier versions lack full C++23 support needed for our codebase
- **Rust Integration**: 23MB static library (libgoldcoin_ffi.a) must be linked
- **Mount Shasta Trinity**: This build environment powers the world's first C++23/Rust hybrid cryptocurrency

---
*"Building the future with tomorrow's tools today!"* 🚀