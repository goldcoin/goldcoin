# Goldcoin Core v17 Linux/Unix Build Guide

**How to build Goldcoin Core v17 on Linux/Unix systems using CMake**

This guide covers building Goldcoin Core v17 for Linux using the traditional CMake approach with hybrid toolchain architecture.

## Requirements

### System Requirements
- **Ubuntu 22.04+** or equivalent Linux distribution
- **8GB RAM** recommended for parallel compilation  
- **20GB disk space** for dependencies and build artifacts
- **System GCC 13+** for full C++23 support

### Essential System Packages
```bash
sudo apt update && sudo apt upgrade
sudo apt install build-essential pkg-config curl git cmake

# Install GCC 13 if not available (Ubuntu 22.04)
sudo apt install gcc-13 g++-13
```

### Hybrid Toolchain Architecture

The Linux build system uses a **hybrid approach**:
- **System compilers**: GCC 13 (for C++23 support)
- **System CMake**: 3.22+ (from distribution)
- **Depends libraries**: All dependencies built statically (OpenSSL 3.5.2, BDB 18.1, etc.)

This provides the best of both worlds: modern system tools with controlled, static dependencies.

## Quick Build

### Standard Build Process
```bash
# Clone repository
git clone https://github.com/microguy/goldcoin.git
cd goldcoin

# Build dependencies (one-time setup, ~30-45 minutes)
cd depends
make -j$(nproc)

# Configure and build goldcoin
cd ..
mkdir build && cd build
cmake .. -DCMAKE_C_COMPILER=gcc-13 -DCMAKE_CXX_COMPILER=g++-13
make -j$(nproc)
```

### Build Outputs
This produces statically linked binaries in `build/bin/`:
- **`goldcoind`** - Full node daemon/server
- **`goldcoin-cli`** - Command line interface  
- **`goldcoin-tx`** - Transaction utility

## Dependencies

### Modern Dependencies (v17)
Goldcoin v17 uses current, secure versions:

| Component | Version | Purpose |
|-----------|---------|---------|
| **OpenSSL** | 3.5.2 | Cryptographic operations (modern, secure) |
| **BerkeleyDB** | 18.1 | Wallet database (latest with improvements) |
| **libevent** | 2.1.12 | Network event handling |
| **Qt6** | 6.9.0 | GUI framework (optional, first Qt 6.9 crypto wallet) |
| **System GCC** | 13+ | C++23 compiler (from Ubuntu/distribution) |
| **System CMake** | 3.22+ | Build system (from Ubuntu/distribution) |

All dependencies are built from source for maximum security and reproducibility.

## Static Linking

### Verification
Goldcoin v17 produces **truly static binaries** by default:

```bash
# Verify static linking
ldd build/bin/goldcoind
# Expected output: "not a dynamic executable"

file build/bin/goldcoind  
# Expected output: "statically linked"

# Check binary size
ls -lh build/bin/goldcoind
# Expected: ~11MB (with debug symbols), ~9MB (stripped)
```

### Benefits
- ✅ **No runtime dependencies** - runs on any Linux system
- ✅ **Portable** across distributions and versions
- ✅ **Enterprise deployment ready** - no library conflicts
- ✅ **Consistent behavior** across different systems

## Build Configuration Options

### GUI Build (Optional)
```bash
# Build with Qt6 GUI wallet
cd build
cmake .. -DCMAKE_C_COMPILER=gcc-13 -DCMAKE_CXX_COMPILER=g++-13 -DBUILD_GUI=ON
make -j$(nproc)
# Produces: goldcoin-qt (Qt 6.9 GUI wallet)
```

### Wallet Disabled Build
```bash
# Server-only build (no wallet functionality)
cd build  
cmake .. -DCMAKE_C_COMPILER=gcc-13 -DCMAKE_CXX_COMPILER=g++-13 -DBUILD_WALLET=OFF
make -j$(nproc)
```

### Debug Build
```bash
# Development build with debug symbols
cd build
cmake .. -DCMAKE_C_COMPILER=gcc-13 -DCMAKE_CXX_COMPILER=g++-13 -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
```

### Release Build (Default)
```bash
# Optimized production build
cd build
cmake .. -DCMAKE_C_COMPILER=gcc-13 -DCMAKE_CXX_COMPILER=g++-13 -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

## Depends System

### Directory Structure
The `depends/` directory manages all dependencies:

```
depends/
├── packages/                    # Package definitions (.mk files)
├── sources/                     # Downloaded source archives
├── work/                        # Build workspace (temporary)
├── built/                       # Cached built packages
└── x86_64-pc-linux-gnu/        # Final installed dependencies
```

### Depends Build Process
```bash
# First-time dependency build (~30-45 minutes)
cd depends
make -j$(nproc)

# Subsequent builds (incremental, much faster)
make -j$(nproc)    # Only rebuilds changed components

# Force clean rebuild of all dependencies
make clean-all
make -j$(nproc)
```

### Job Control
```bash
# Auto-detect CPU cores (recommended)
make -j$(nproc)

# Specify job count manually
make -j4          # 4 parallel jobs
make -j8          # 8 parallel jobs  
make -j16         # 16 parallel jobs (powerful machines)
```

## Advanced Options

### Memory Optimization
```bash
# For systems with limited RAM (<8GB)
cd depends
make -j2          # Reduce parallel jobs

cd ../build  
make -j2          # Reduce compilation jobs
```

### Clean Builds
```bash
# Clean CMake build artifacts only
rm -rf build
mkdir build && cd build

# Clean everything and rebuild from scratch
rm -rf build depends/x86_64-pc-linux-gnu depends/work
cd depends && make -j$(nproc)
cd .. && mkdir build && cd build && cmake .. && make -j$(nproc)
```

### Cross-Compilation (Advanced)
```bash
# Example: Build for ARM64
cd depends  
make HOST=aarch64-linux-gnu -j$(nproc)

cd .. && mkdir build-arm64 && cd build-arm64
cmake .. -DCMAKE_TOOLCHAIN_FILE=../depends/aarch64-linux-gnu/share/toolchain.cmake
make -j$(nproc)
```

## Verification & Testing

### Basic Testing
```bash
# Test binary execution
./build/bin/goldcoind --version
./build/bin/goldcoin-cli --help

# Start daemon (testnet recommended for testing)
./build/bin/goldcoind -testnet -daemon

# Check status
./build/bin/goldcoin-cli -testnet getblockchaininfo

# Stop daemon
./build/bin/goldcoin-cli -testnet stop
```

### Security Verification
```bash
# Check security hardening (if hardening-check is available)
hardening-check build/bin/goldcoind

# Verify static linking (no external dependencies)
ldd build/bin/goldcoind

# Check for debugging symbols
file build/bin/goldcoind
nm build/bin/goldcoind | wc -l  # Should be 0 for stripped release build
```

## Installation

### System Installation (Optional)
```bash
# Install to /usr/local/bin (requires sudo)
sudo make install

# Install to custom directory
make install DESTDIR=/opt/goldcoin
```

### Manual Installation
```bash
# Copy binaries to desired location
mkdir -p ~/goldcoin/bin
cp build/bin/goldcoin* ~/goldcoin/bin/

# Create configuration directory
mkdir -p ~/.goldcoin
```

## Troubleshooting

### Build Issues

**"No space left on device":**
- Ensure 20GB+ free space available
- Clean old builds: `rm -rf build depends/work`
- Check `/tmp` space (used during compilation)

**"Out of memory" errors:**
- Reduce parallel jobs: `make -j2` instead of `make -j$(nproc)`
- Add swap space if available
- Close other memory-intensive applications

**GCC version too old:**
```bash
# Install GCC 13 on Ubuntu 22.04
sudo apt install gcc-13 g++-13

# Verify version
gcc-13 --version  # Should show 13.x.x
```

**CMake version too old:**
```bash
# Check version
cmake --version  # Should be 3.21+

# Update cmake if needed (Ubuntu 22.04 has 3.22)
sudo apt update && sudo apt upgrade cmake
```

### Dependency Issues

**Download failures:**
- Check internet connection
- Verify DNS resolution: `nslookup bitcoincore.org`
- Source archives are cached in `depends/sources/`
- Manually download if needed

**Hash verification failures:**
```bash
# Clean and retry specific package
rm -rf depends/work/build/x86_64-pc-linux-gnu/[package]*
rm -rf depends/built/x86_64-pc-linux-gnu/[package]*
cd depends && make -j$(nproc)
```

### Runtime Issues

**"Permission denied" when running binaries:**
```bash
chmod +x build/bin/goldcoind build/bin/goldcoin-cli build/bin/goldcoin-tx
```

**Configuration errors:**
```bash
# Create default config directory
mkdir -p ~/.goldcoin

# Basic configuration file
cat > ~/.goldcoin/goldcoin.conf << EOF
# Basic Goldcoin configuration
server=1
rpcuser=goldcoinrpc
rpcpassword=your_secure_password_here
rpcallowip=127.0.0.1
daemon=1
EOF
```

## Security Considerations

### Build Security
- **Use clean, isolated build environment** for production releases
- **Verify source archive checksums** in `depends/sources/`
- **Review dependency versions** for security updates
- **Use deterministic builds** when possible

### Runtime Security  
- **Enable firewall protection** for network services
- **Use strong RPC passwords** for daemon access
- **Run with least privilege** (non-root user recommended)
- **Keep system updated** with security patches

### Hardening (Enabled by Default)
- ✅ **Position Independent Executable (PIE)**
- ✅ **Stack protection** (`-fstack-protector-all`)
- ✅ **Address Space Layout Randomization (ASLR)**
- ✅ **Static linking** eliminates runtime dependency attacks

## Technical Details

### C++23 Features
Goldcoin v17 uses modern C++23 language features:
- **`std::expected`** for error handling
- **Concepts** for type safety
- **Ranges and views** for data processing
- **Coroutines** for async operations
- **Modern memory management** patterns

### Build System Architecture
- **CMake 3.22+**: Modern, cross-platform build system
- **Static linking**: Zero runtime dependencies
- **Hybrid toolchain**: System compilers + controlled dependencies  
- **Parallel builds**: Automatic CPU detection with job control
- **Incremental compilation**: Only rebuild changed components

---

**This build system is designed for enterprise use with security, reproducibility, and performance as primary goals.**