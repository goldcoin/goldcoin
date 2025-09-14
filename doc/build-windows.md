# Goldcoin Core v17 Windows Build Guide

**Enterprise-grade cross-compilation system for Windows binaries**

This guide covers building Goldcoin Core v17 for Windows using the modern enterprise build system with separation of concerns, hybrid toolchain architecture, and C++23 support.

## System Requirements

### Recommended Environment
- **WSL2** (Windows Subsystem for Linux 2) with Ubuntu 22.04+
- **8+ CPU cores** for reasonable build times
- **16+ GB RAM** for parallel compilation
- **50+ GB free space** for dependencies and build artifacts

### Enterprise Toolchain Architecture

The build system uses a **hybrid approach**:
- **System compilers**: GCC 13 (native) + MinGW GCC 13 (cross-compiler)
- **Depends libraries**: All dependencies built statically (OpenSSL 3.5.2, BDB 18.1, Qt 6.9)
- **Two-phase process**: Dependency building → Binary compilation (separation of concerns)

## Initial Setup

### 1. Install WSL2 (if needed)

```powershell
# Run in PowerShell as Administrator
wsl --install Ubuntu-22.04
```

Complete Ubuntu setup and create user account when prompted.

### 2. System Toolchain Installation

**Essential packages:**
```bash
sudo apt update && sudo apt upgrade
sudo apt install build-essential pkg-config curl git cmake ninja-build
```

**Install Ubuntu 24.04 backports for MinGW GCC 13:**
```bash
# Add Noble (24.04) backports repository
sudo add-apt-repository "deb http://archive.ubuntu.com/ubuntu noble main universe"
sudo apt update

# Install MinGW GCC 13 (required for C++23 support)
sudo apt install gcc-mingw-w64 g++-mingw-w64 gcc-mingw-w64-x86-64-posix g++-mingw-w64-x86-64-posix
```

**Configure MinGW for POSIX threading:**
```bash
sudo update-alternatives --config x86_64-w64-mingw32-g++
# Choose: x86_64-w64-mingw32-g++-posix
```

**Verify installation:**
```bash
# Should show GCC 13-posix (required for C++23 <expected> headers)
x86_64-w64-mingw32-gcc-posix --version

# Should show GCC 13
gcc-13 --version
```

## Enterprise Build System

### Build Process: Separation of Concerns

The enterprise build system implements clean **two-phase architecture**:

#### Phase 1: Dependency Building
```bash
cd build
make deps-windows-no-qt JOBS=8    # For command-line only
# OR
make deps-windows JOBS=8          # For GUI (includes Qt 6.9)
```

**What this builds:**
- OpenSSL 3.5.2 (cryptographic operations)
- BerkeleyDB 18.1 (wallet database)
- libevent 2.1.12 (network event handling)  
- ZeroMQ 4.3.4 (message queuing)
- miniupnpc 2.2.2 (UPnP support)
- Qt 6.9.0 (GUI framework, if requested)

**Output:** Static libraries in `depends/x86_64-w64-mingw32/`

#### Phase 2: Binary Compilation
```bash
cd build
make windows JOBS=8               # Command-line binaries
# OR  
make windows-qt JOBS=8            # GUI binary
```

**What this builds:**
- Uses pre-built dependencies (read-only access)
- Compiles Goldcoin Core C++23 codebase
- Links everything statically for Windows

**Output:** Windows executables in `build/bin/windows/`

### Why Separation of Concerns?

**Before**: Mixed phases caused:
- 40+ minute dependency rebuilds during binary compilation
- Cache invalidation from build system changes
- Cross-contamination between dependency building and binary compilation

**After**: Clean separation ensures:
- Dependencies build once, use many times
- Fast binary rebuilds (minutes, not hours)  
- Stable dependency cache
- Enterprise-grade predictability

## Quick Start Commands

```bash
# Clone repository
git clone https://github.com/microguy/goldcoin.git
cd goldcoin/build

# Check build system status
make status

# Build Windows dependencies (one-time, ~60 minutes)
make deps-windows-no-qt JOBS=8

# Build Windows binaries (fast, ~5 minutes)
make windows JOBS=8

# Binaries ready at: build/bin/windows/
ls -la bin/windows/
```

## Build Outputs

### Command-Line Binaries
**Location:** `build/bin/windows/`

- **`goldcoind.exe`** - Windows daemon/server
- **`goldcoin-cli.exe`** - Command-line interface  
- **`goldcoin-tx.exe`** - Transaction utility

### GUI Binary (Optional)
**Location:** `build/bin/windows-qt/`

- **`goldcoin-qt.exe`** - Qt 6.9 GUI wallet

All binaries are **fully static** with no external dependencies.

## Advanced Options

### Job Control
```bash
# Auto-detect CPU cores (default)
make windows

# Specify job count
make windows JOBS=4              # 4 parallel jobs
make windows JOBS=16             # 16 parallel jobs (powerful machines)
```

### Clean Builds
```bash
make clean                       # Clean build artifacts (preserve dependencies)
make status                      # Check what needs rebuilding
```

### Debug Builds
```bash
# For development/debugging (not production)
cd build/windows
cmake ../.. -DCMAKE_BUILD_TYPE=Debug \
           -DCMAKE_TOOLCHAIN_FILE=../../depends/x86_64-w64-mingw32/share/toolchain.cmake
make -j8
```

## Verification

### Check Binary Dependencies
```bash
# Should show only system32 DLLs (no external dependencies)
x86_64-w64-mingw32-objdump -p build/bin/windows/goldcoind.exe | grep DLL
```

### Test Execution (Wine)
```bash
# Install Wine for testing
sudo apt install wine64

# Test Windows binary on Linux
wine build/bin/windows/goldcoind.exe --version
wine build/bin/windows/goldcoin-cli.exe --help
```

## Installation on Windows

### Copy to Windows
```bash
# Via WSL2 Windows mount
mkdir -p /mnt/c/Goldcoin/bin
cp build/bin/windows/*.exe /mnt/c/Goldcoin/bin/

# Via network copy, USB, etc.
scp build/bin/windows/*.exe user@windows-machine:/path/to/goldcoin/
```

### Windows Configuration
**Create goldcoin.conf:**
```ini
# Location: C:\Users\%USERNAME%\AppData\Roaming\Goldcoin\goldcoin.conf
server=1
rpcuser=goldcoinrpc  
rpcpassword=your_secure_password_here
rpcallowip=127.0.0.1
daemon=1
```

### Windows Service (Optional)
```cmd
# Install as Windows service (Run as Administrator)
goldcoind.exe -install

# Start service
net start "Goldcoin Core"

# Check status
goldcoin-cli.exe getblockchaininfo
```

## Troubleshooting

### MinGW Version Issues
**Problem:** `<expected>` header not found, C++23 compilation errors

**Solution:** Verify MinGW GCC 13 installation:
```bash
x86_64-w64-mingw32-gcc-posix --version
# Must show: GCC 13-posix
```

If showing GCC 10, reinstall Ubuntu 24.04 backports as shown above.

### WSL2 Cross-Compilation Popups
**Problem:** Windows DLL error popups during dependency building

**Solution:** Temporarily disable Windows interoperability:
```bash
# Before building dependencies  
echo 0 | sudo tee /proc/sys/fs/binfmt_misc/WSLInterop

# Build dependencies
make deps-windows-no-qt JOBS=8

# Re-enable after completion
echo 1 | sudo tee /proc/sys/fs/binfmt_misc/WSLInterop
```

### Memory Issues
**Problem:** "out of memory" errors during compilation

**Solutions:**
```bash
# Reduce parallel jobs
make windows JOBS=2

# Or increase WSL2 memory (edit .wslconfig)
# C:\Users\%USERNAME%\.wslconfig
[wsl2]
memory=8GB
```

### Path Issues
**Problem:** Build fails with path-related errors

**Solution:** Ensure source is in WSL filesystem:
```bash
# Good: /home/user/goldcoin (WSL filesystem)
# Bad:  /mnt/c/goldcoin (Windows mount)
```

### Dependency Download Failures
**Problem:** Cannot download source packages

**Solutions:**
1. Check internet connectivity in WSL2
2. Source archives cached in `depends/sources/`
3. Manually download missing packages if needed
4. Verify DNS resolution: `nslookup bitcoincore.org`

## Security Considerations

### Code Signing
- **Production releases**: Sign with valid Windows code signing certificate
- **Antivirus**: Some AV software flags unsigned crypto binaries (normal for development builds)
- **Firewall**: Windows may prompt for network access permissions

### Build Security
- Build on isolated/dedicated systems for production
- Verify source archive checksums in `depends/sources/`
- Use deterministic builds when possible
- Review dependency versions for security updates

## Technical Details

### Modern Features
- **C++23 support**: Uses modern language features (`<expected>`, concepts, ranges)
- **OpenSSL 3.5.2**: Latest cryptographic library with enhanced security
- **Qt 6.9**: Modern GUI framework (first Qt 6.9 crypto wallet)
- **BDB 18.1**: Latest Berkeley Database with improved Windows file locking

### Build System Architecture
- **CMake 3.21+**: Modern build system generator
- **Ninja**: Fast parallel build execution (when available)
- **Static linking**: Zero-dependency Windows executables  
- **Cross-compilation**: Linux host → Windows target
- **Hybrid toolchain**: System compilers + depends libraries

### Performance
- **Parallel builds**: Automatic CPU detection with job control
- **Incremental builds**: Only rebuild changed components
- **Dependency caching**: Build dependencies once, use many times
- **Smart detection**: Auto-detect available tools and paths

---

**This enterprise build system is designed for production use with clean architecture, separation of concerns, and proven toolchain compatibility.**