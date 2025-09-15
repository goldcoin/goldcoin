# Gold Coin Enterprise Build System Guide

## Overview

This document provides a comprehensive guide to the Gold Coin enterprise build system. This system is designed for clean, enterprise-level builds for end users, separate from the development environment.

## System Architecture

### Two-Tier Build Environment

1. **Enterprise Server (Hetzner VPS)**: Production build environment with isolated build system (outside of depends)
2. **Development Server**: Local development environment where we're implementing full depends suite builds for the first time

### Key Directories

```
goldcoin/
├── build/                    # Main build directory - ALL commands run from here
│   ├── GNUmakefile          # Enterprise build system entry point
│   ├── bin/
│   │   ├── linux/           # Linux binaries (goldcoind, goldcoin-cli, etc.)
│   │   └── windows/         # Windows binaries (.exe files)
│   └── [build artifacts]
├── depends/                 # Dependencies build system
│   ├── Makefile            # Core depends build logic
│   ├── packages/           # Package definitions
│   ├── funcs.mk           # Build functions and macros
│   └── x86_64-pc-linux-gnu/   # Linux deps (working)
└── src/                    # Source code
```

## First Time Setup

### Step 1: Build Dependencies (Required)

**For Linux builds:**
```bash
make deps-linux
```

**For Windows builds (without Qt GUI):**
```bash
make deps-windows-no-qt
```

**For Windows builds (with Qt GUI):**
```bash
make deps-windows
```

**Note:** Dependencies only need to be built once and are preserved between builds.

### Step 2: Build Binaries

```bash
make linux              # Linux binaries → build/linux/bin/
make windows             # Windows binaries → build/windows/bin/
make windows-qt          # Windows GUI → build/windows-qt/bin/
```

## Enterprise Build System Commands

### Available Targets (run from repository root)

- `make linux` - Build Linux binaries
- `make windows` - Build Windows binaries  
- `make windows-qt` - Build Windows binaries with Qt GUI
- `make deps-linux` - Build Linux dependencies only
- `make deps-windows` - Build Windows dependencies with Qt
- `make deps-windows-no-qt` - Build Windows dependencies without Qt
- `make status` - Show build status

### Build Flow Logic: Separation of Concerns

The build system implements a clean **two-phase approach** to prevent cross-contamination:

#### Phase 1: Dependency Building
1. **User Command**: `make deps-windows-no-qt` (from repository root)
2. **GNUmakefile**: Calls `make -C depends HOST=x86_64-w64-mingw32 NO_QT=1 -j8`
3. **Depends Makefile**: 
   - Target: `all` → `install` → `$(host_prefix)/share/config.site`
   - Dependency: `$(host_prefix)/.stamp_$(final_build_id)`
   - Requires: All `$(native_packages)` + `$(packages)` built and cached
4. **Package Build Order**:
   - Native packages: native_ccache, native_gcc
   - Target packages: openssl, libevent, zeromq, bdb, miniupnpc
5. **Final Output**: `depends/x86_64-w64-mingw32/` with complete toolchain

#### Phase 2: Binary Compilation
1. **User Command**: `make windows` or `make windows-qt` (from repository root)
2. **Dependency Verification**: Checks for required libraries, fails fast if missing
3. **Read-Only Access**: Uses existing `depends/` libraries without modification
4. **CMake Build**: Compiles Goldcoin Core binaries using pre-built dependencies
5. **Final Output**: Binaries in `build/windows/bin/` or `build/windows-qt/bin/`

#### Critical Engineering Principle
**Binary compilation commands cannot modify the depends system.** This prevents:
- Cache invalidation from build system changes
- Accidental dependency rebuilds during binary compilation
- Cross-contamination between dependency building and binary compilation phases
- 40+ minute OpenSSL rebuilds when only compiling binaries

The `make windows` and `make windows-qt` commands have **read-only** access to `depends/` and will fail immediately if required dependencies are missing, directing users to run the appropriate `make deps-*` command first.

## Current Status

### ✅ Working Systems
- **Linux Dependencies**: Complete and functional at `depends/x86_64-pc-linux-gnu/`
- **Linux Builds**: Full working system with GCC 13 and C++23
- **Enterprise Server**: Ubuntu 24.04 with MinGW GCC 13, building successfully for months
- **Windows Dependencies**: OpenSSL cache invalidation issues completely resolved
- **Separation of Concerns**: Build system architecture prevents cross-contamination
- **Hybrid Toolchain**: System compilers + depends libraries architecture proven

### 🔄 In Progress  
- **MinGW GCC 13 Installation**: Upgrading from GCC 10 to GCC 13 via Ubuntu 24.04 backports
- **C++23 Windows Support**: Waiting for MinGW upgrade to resolve `<expected>` header issues

### 📋 System Requirements
**For C++23 Support (Required):**
- **Ubuntu 22.04+** with Noble (24.04) backports repository
- **MinGW GCC 13-posix** (not the default GCC 10 from Ubuntu 22.04)
- **System GCC 13** for Linux builds

**Installation:**
```bash
# Add Ubuntu 24.04 backports
sudo add-apt-repository "deb http://archive.ubuntu.com/ubuntu noble main universe"
sudo apt update
sudo apt install gcc-mingw-w64 g++-mingw-w64 gcc-mingw-w64-x86-64-posix g++-mingw-w64-x86-64-posix
```

## Package Definitions

### Windows NO_QT Build Packages

**Hybrid Toolchain Architecture:**
- **System Compilers**: MinGW GCC 13-posix (from Ubuntu 24.04 backports)
- **Depends Libraries**: All dependencies built statically in `depends/x86_64-w64-mingw32/`

**Native Packages** (build tools):
- `native_ccache`: Compiler cache for faster rebuilds

**Target Packages** (runtime dependencies):
- `openssl`: OpenSSL 3.5.2 (cryptographic operations)
- `libevent`: libevent 2.1.12 (network event handling)
- `zeromq`: ZeroMQ 4.3.4 (message queuing) 
- `bdb`: BerkeleyDB 18.1 (wallet database with Windows file lock fixes)
- `miniupnpc`: miniUPnPc 2.2.2 (UPnP port mapping)

**Note:** Unlike previous versions, we do NOT build `native_gcc` - the system provides MinGW GCC 13 cross-compiler, while depends provides all static libraries.

## Build System Components

### Core Files
- `depends/Makefile`: Main build orchestration
- `depends/funcs.mk`: Build functions and extraction logic
- `depends/packages/packages.mk`: Package inclusion logic
- `build/GNUmakefile`: Enterprise build entry points

### Key Functions (funcs.mk)
- Package downloading and verification
- Source extraction with hash validation
- Caching system for built packages
- Cross-compilation setup

## Troubleshooting Guide

### Common Issues

1. **C++23 Header Missing (`<expected>` not found)**
   - Cause: MinGW GCC 10 doesn't support C++23
   - Solution: Install MinGW GCC 13 via Ubuntu 24.04 backports (see System Requirements above)
   - Verify: `x86_64-w64-mingw32-gcc-posix --version` should show "GCC 13-posix"
   
2. **Windows Dependencies Missing**
   - Check: Run `make status` from build/ directory
   - Solution: Run `make deps-windows-no-qt JOBS=8` first
   - Verify: `ls depends/x86_64-w64-mingw32/lib/libcrypto.a` should exist

3. **Hash Mismatches** (Rare)
   - Cause: Corrupted downloaded files or build modifications
   - Solution: Run `make linux`, `make windows`, or `make windows-qt` (clean builds by default will rebuild everything fresh)

### Debug Commands

```bash
# Check build status
make -C depends HOST=x86_64-w64-mingw32 NO_QT=1 -n

# Verify cache state  
ls -la depends/built/x86_64-w64-mingw32/

# Check current build state
ls -la depends/x86_64-w64-mingw32/

# Monitor build progress
make -C depends HOST=x86_64-w64-mingw32 NO_QT=1 V=1
```

## Critical Guidelines for Claude Code

### DO NOT:
- Add log files to the repository (use /tmp/ for logs)
- Jump into action without understanding the system first
- Make patches during active builds (invalidates cache)  
- Use system links or hacks for enterprise builds
- Modify files without permission and discussion

### DO:
- Study the existing system architecture first
- Understand the complete build flow before changes
- Ask for permission before making modifications
- Focus on clean enterprise-level solutions
- Preserve the carefully optimized 8MB repository size

### Investigation Priority
1. Understand depends system architecture
2. Study package definitions and build processes  
3. Analyze caching and dependency system
4. Review toolchain configuration files
5. Examine cross-compilation setup
6. Study error patterns and build flows

## Repository Notes

- **Size**: Carefully reduced from 1GB+ to 8MB through extensive optimization
- **Cleanliness**: Pristine enterprise-ready state
- **Purpose**: End-user enterprise build system (not development hacks)
- **Working State**: Linux builds fully functional, Windows builds in progress

## Emergency Recovery

If builds become corrupted:
1. Check git status for modifications
2. Consider reverting to last known working state
3. Clean build directories: Use `make -C depends clean-sources` or restart depends build
4. Preserve cached packages: `depends/built/` if valid
5. Restart from clean state

---

**Remember**: This is an enterprise build system for end users. It should work cleanly without hacks, workarounds, or development shortcuts. Every change should maintain the professional, clean architecture designed for production use.