UNIX BUILD NOTES
====================
How to build Goldcoin Core v17 on Unix systems using CMake.

Requirements
---------------------
- **CMake 4.1+** (provided by depends system)
- **GCC 15.2+** (provided by depends system) 
- **8GB RAM** recommended for parallel compilation
- **20GB disk space** for full depends build

Quick Build
---------------------

```bash
# Build dependencies (one-time setup)
cd depends
make -j$(nproc)

# Configure and build goldcoin
cd ..
mkdir build && cd build
cmake ..
make -j$(nproc)
```

This produces statically linked binaries in `build/bin/`:
- `goldcoind` - Full node daemon
- `goldcoin-cli` - Command line interface  
- `goldcoin-tx` - Transaction utility

Enterprise Dependencies
---------------------

Goldcoin v17 uses a **depends system** for reproducible enterprise builds:

| Component | Version | Purpose |
|-----------|---------|---------|
| OpenSSL | 1.0.1k | Cryptographic operations |
| libevent | 2.1.8 | Network event handling |
| BerkeleyDB | 18.1 | Wallet storage |
| Qt6 | 6.9.0 | GUI framework (optional) |
| GCC | 15.2 | C++23 compiler |
| CMake | 4.1.0 | Build system |

All dependencies are built from source for maximum compatibility and security.

Static Linking
---------------------

Goldcoin v17 produces **truly static binaries** by default:

```bash
# Verify static linking
ldd build/bin/goldcoind
# Output: "not a dynamic executable"

file build/bin/goldcoind  
# Output: "statically linked"
```

**Benefits:**
- No runtime dependencies
- Portable across Linux distributions
- Enterprise deployment ready
- Consistent behavior across systems

**Size:** ~11MB (with debug symbols), ~9MB (stripped)

Build Configuration
---------------------

Common CMake options:

```bash
# GUI build with Qt6
cmake .. -DBUILD_GUI=ON

# Disable wallet functionality  
cmake .. -DBUILD_WALLET=OFF

# Release build (default)
cmake .. -DCMAKE_BUILD_TYPE=Release

# Debug build with symbols
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

System Requirements: Ubuntu/Debian
---------------------

**Minimal system dependencies:**
```bash
sudo apt-get update
sudo apt-get install build-essential pkg-config libevent-dev
```

**Note:** The depends system builds all other requirements (GCC 15.2, CMake 4.1, etc.) automatically.

Depends System
---------------------

The `depends/` directory contains:

```
depends/
├── packages/          # Package definitions
├── sources/          # Source archives  
└── x86_64-pc-linux-gnu/  # Built dependencies
```

**First-time setup:**
```bash
cd depends
make -j$(nproc)    # Build all dependencies (~20-30 minutes)
```

**Subsequent builds:**
```bash
make -j$(nproc)    # Only rebuilds changed components
```

Advanced Build Options
---------------------

**Clean rebuild:**
```bash
rm -rf build depends/x86_64-pc-linux-gnu
cd depends && make -j$(nproc)
cd .. && mkdir build && cd build && cmake .. && make -j$(nproc)
```

**Cross-compilation example (ARM):**
```bash
cd depends  
make HOST=arm-linux-gnueabihf -j$(nproc)
cd .. && mkdir build-arm && cd build-arm
cmake .. -DCMAKE_TOOLCHAIN_FILE=../depends/toolchain-arm.cmake
make -j$(nproc)
```

**Memory optimization:**
```bash
# For systems with <8GB RAM
make -j2    # Reduce parallel jobs
```

Verification
---------------------

**Test the build:**
```bash
# Start daemon
./build/bin/goldcoind

# Check status  
./build/bin/goldcoin-cli getinfo

# Stop daemon
./build/bin/goldcoin-cli stop
```

**Security verification:**
```bash
# Check hardening flags
hardening-check build/bin/goldcoind

# Verify no dynamic dependencies
ldd build/bin/goldcoind
```

Troubleshooting
---------------------

**Build fails with "No space left":**
- Ensure 20GB+ free space
- Clean old builds: `rm -rf build depends/work`

**Memory issues during compilation:**  
- Reduce parallel jobs: `make -j2`
- Enable swap if available

**Dependency download fails:**
- Check internet connection
- Source archives cached in `depends/sources/`

**CMake version too old:**
- The depends system builds CMake 4.1.0 automatically
- Do not use system CMake

Security Notes
---------------------

**Hardening (enabled by default):**
- Position Independent Executable (PIE)
- Stack protection
- Address Space Layout Randomization (ASLR)
- Static linking eliminates runtime dependencies

**Reproducible builds:**
- All dependencies built from fixed source versions
- Deterministic compilation flags
- Enterprise-grade consistency

For additional security, run on isolated build systems and verify checksums of source archives in `depends/sources/`.