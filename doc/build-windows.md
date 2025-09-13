WINDOWS BUILD NOTES
====================

Building Goldcoin Core v17 for Windows using cross-compilation.

Overview
--------

Goldcoin Core Windows builds use **cross-compilation from Linux** with the CMake enterprise build system. This produces statically linked Windows executables with no runtime dependencies.

**Recommended environment:** WSL2 (Windows Subsystem for Linux 2) with Ubuntu 22.04+

Cross-Compilation Setup
-----------------------

### Install WSL2 (if not already installed)

1. **Enable WSL2:**
   ```powershell
   # Run in PowerShell as Administrator
   wsl --install Ubuntu-22.04
   ```

2. **Complete Ubuntu setup:**
   ```bash
   # Create UNIX user account when prompted
   # Update system packages
   sudo apt update && sudo apt upgrade
   ```

### Install Build Dependencies

**Essential packages:**
```bash
sudo apt-get install build-essential pkg-config curl git
sudo apt-get install g++-mingw-w64-x86-64 mingw-w64-tools
```

**Configure mingw compiler:**
```bash
# Set to posix threading model
sudo update-alternatives --config x86_64-w64-mingw32-g++
# Choose: x86_64-w64-mingw32-g++-posix
```

Building for Windows 64-bit
----------------------------

### Quick Build

```bash
# Build Windows dependencies (one-time setup)
cd depends
make HOST=x86_64-w64-mingw32 -j$(nproc)

# Configure and build
cd ..
mkdir build-win && cd build-win
cmake .. -DCMAKE_TOOLCHAIN_FILE=../depends/x86_64-w64-mingw32/toolchain.cmake
make -j$(nproc)
```

**Output location:** `build-win/bin/`
- `goldcoind.exe` - Windows daemon
- `goldcoin-cli.exe` - Command line interface
- `goldcoin-tx.exe` - Transaction utility

### Windows Dependencies

The depends system builds all Windows requirements:

| Component | Version | Purpose |
|-----------|---------|---------|
| OpenSSL | 1.0.1k | Cryptographic operations |
| libevent | 2.1.8 | Network event handling |
| BerkeleyDB | 18.1 | Wallet storage |
| Qt6 | 6.9.0 | GUI framework |
| MinGW-w64 | Latest | Cross-compilation toolchain |

### GUI Build (Optional)

```bash
# Build with Qt6 GUI
cd build-win
cmake .. -DCMAKE_TOOLCHAIN_FILE=../depends/x86_64-w64-mingw32/toolchain.cmake \
         -DBUILD_GUI=ON
make -j$(nproc)
```

This produces `goldcoin-qt.exe` in addition to command-line tools.

Windows Binary Features
-----------------------

**Static linking:** All Windows binaries are fully static with no external dependencies.

**Verification:**
```bash
# Check binary dependencies (should show only system32 DLLs)
x86_64-w64-mingw32-objdump -p build-win/bin/goldcoind.exe | grep DLL
```

**Security hardening:**
- ASLR (Address Space Layout Randomization)
- DEP (Data Execution Prevention) 
- Control Flow Guard
- Stack protection

Installation on Windows
-----------------------

### Manual Installation

Copy binaries to Windows directory:
```bash
# Create Windows installation directory
mkdir -p /mnt/c/Goldcoin/bin

# Copy executables
cp build-win/bin/*.exe /mnt/c/Goldcoin/bin/
```

### Automatic Installation

```bash
# Install to Windows drive
make install DESTDIR=/mnt/c/Goldcoin
```

### Windows Service Setup

**Create goldcoin.conf:**
```ini
# C:\Users\%USERNAME%\AppData\Roaming\Goldcoin\goldcoin.conf
server=1
rpcuser=goldcoinrpc
rpcpassword=your_secure_password_here
rpcallowip=127.0.0.1
```

**Run as Windows service:**
```cmd
# Install service (Run as Administrator)
goldcoind.exe -install

# Start service  
net start "Goldcoin Core"
```

Building for Windows 32-bit
----------------------------

**Install 32-bit toolchain:**
```bash
sudo apt-get install g++-mingw-w64-i686 mingw-w64-i686-dev
sudo update-alternatives --config i686-w64-mingw32-g++
```

**Build process:**
```bash
cd depends
make HOST=i686-w64-mingw32 -j$(nproc)
cd ..
mkdir build-win32 && cd build-win32
cmake .. -DCMAKE_TOOLCHAIN_FILE=../depends/i686-w64-mingw32/toolchain.cmake
make -j$(nproc)
```

Advanced Options
----------------

### Custom Build Configuration

```bash
# Release build (default)
cmake .. -DCMAKE_BUILD_TYPE=Release \
         -DCMAKE_TOOLCHAIN_FILE=../depends/x86_64-w64-mingw32/toolchain.cmake

# Debug build with symbols
cmake .. -DCMAKE_BUILD_TYPE=Debug \
         -DCMAKE_TOOLCHAIN_FILE=../depends/x86_64-w64-mingw32/toolchain.cmake

# Wallet disabled
cmake .. -DBUILD_WALLET=OFF \
         -DCMAKE_TOOLCHAIN_FILE=../depends/x86_64-w64-mingw32/toolchain.cmake
```

### Clean Rebuild

```bash
# Remove all build artifacts
rm -rf build-win depends/x86_64-w64-mingw32

# Full rebuild
cd depends
make HOST=x86_64-w64-mingw32 -j$(nproc)
cd .. && mkdir build-win && cd build-win
cmake .. -DCMAKE_TOOLCHAIN_FILE=../depends/x86_64-w64-mingw32/toolchain.cmake
make -j$(nproc)
```

Troubleshooting
---------------

### WSL Path Issues

**Problem:** Build fails with path-related errors.

**Solution:** Ensure source code is in WSL filesystem, not Windows mount:
```bash
# Good: /home/user/goldcoin
# Bad:  /mnt/c/goldcoin
```

### Memory Issues

**Problem:** Compilation fails with "out of memory" errors.

**Solution:** Reduce parallel jobs:
```bash
make -j2  # Instead of -j$(nproc)
```

### Dependency Download Failures

**Problem:** Cannot download source packages.

**Solution:** 
1. Check internet connectivity in WSL2
2. Source archives are cached in `depends/sources/`
3. Manually download if needed

### MinGW Compiler Issues

**Problem:** ABI compatibility errors.

**Solution:** Verify posix threading model:
```bash
x86_64-w64-mingw32-g++ -v
# Should show: --enable-threads=posix
```

### WSL2 Cross-Compilation Issues

**Problem:** Windows error popups about missing DLLs (libwinpthread-1.dll) during build. Multiple "checking for... no" messages in configure output.

**Cause:** WSL2 attempts to run Windows test executables (conftest.exe) generated during cross-compilation, causing Windows to search for MinGW runtime DLLs.

**Solution:** Temporarily disable WSL Windows interoperability during builds:
```bash
# Before building dependencies
echo 0 | sudo tee /proc/sys/fs/binfmt_misc/WSLInterop

# Build Windows dependencies
cd depends
make HOST=x86_64-w64-mingw32 -j$(nproc)

# Re-enable Windows interop after build completes
echo 1 | sudo tee /proc/sys/fs/binfmt_misc/WSLInterop
```

**Note:** This is harmless to the build process - the configure tests correctly assume "no" for cross-compilation. The popups are just an annoyance, not actual build failures.

Testing Windows Builds
-----------------------

### In Wine (Linux)

```bash
# Install Wine
sudo apt-get install wine64

# Test Windows binary
wine build-win/bin/goldcoind.exe --version
```

### On Windows

Copy binaries to Windows machine and test:
```cmd
goldcoind.exe --version
goldcoin-cli.exe --help
```

Security Notes
--------------

**Code signing:** For production releases, sign Windows executables with valid certificate.

**Antivirus:** Some antivirus software may flag unsigned crypto binaries. This is normal for self-compiled builds.

**Firewall:** Windows Firewall may prompt for network access when running goldcoind.exe.

For maximum security, build on isolated systems and verify source archive checksums in `depends/sources/`.