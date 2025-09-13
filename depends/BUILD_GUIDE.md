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

## Enterprise Build System Commands

### Available Targets (run from build/ directory)

- `make linux` - Build Linux binaries
- `make windows` - Build Windows binaries  
- `make windows-qt` - Build Windows binaries with Qt GUI
- `make deps-linux` - Build Linux dependencies only
- `make deps-windows` - Build Windows dependencies with Qt
- `make deps-windows-no-qt` - Build Windows dependencies without Qt
- `make clean` - Clean build artifacts
- `make status` - Show build status

### Build Flow Logic

1. **User Command**: `make deps-windows-no-qt JOBS=6` (from build/ directory)
2. **GNUmakefile**: Calls `make -C ../depends HOST=x86_64-w64-mingw32 NO_QT=1 -j6`
3. **Depends Makefile**: 
   - Target: `all` → `install` → `$(host_prefix)/share/config.site`
   - Dependency: `$(host_prefix)/.stamp_$(final_build_id)`
   - Requires: All `$(native_packages)` + `$(packages)` built and cached
4. **Package Build Order**:
   - Native packages: native_ccache, native_gcc
   - Target packages: openssl, libevent, zeromq, bdb, miniupnpc
5. **Final Output**: `depends/x86_64-w64-mingw32/` with complete toolchain

## Current Status & Known Issues

### Working Systems
- **Linux Dependencies**: Complete and functional at `depends/x86_64-pc-linux-gnu/`
- **Linux Builds**: Full working system
- **Enterprise Server**: Isolated build system working perfectly

### Current Problem
- **Windows Dependencies**: Failing after ~40 minutes of building
- **Issue Pattern**: Build starts, rebuilds GCC (~40min), hits error, requires patches, cache invalidated, repeat cycle
- **Attempts**: 6+ failed build cycles as of documentation creation
- **Root Cause**: Unknown - waiting for error message to diagnose

## Package Definitions

### Windows NO_QT Build Packages

**Native Packages** (build tools):
- `native_ccache`: Compiler cache for faster rebuilds
- `native_gcc`: Cross-compiler toolchain (GCC 15.2.0)

**Target Packages** (runtime dependencies):
- `openssl`: Cryptography library
- `libevent`: Event notification library  
- `zeromq`: Message queue library
- `bdb`: Berkeley DB (wallet support)
- `miniupnpc`: UPnP client library

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

1. **Repeated GCC Rebuilds**
   - Cause: Cache invalidation due to build modifications
   - Solution: Avoid modifying build files mid-build
   
2. **Tar Extraction Errors**
   - Check: tar commands have proper -C directory specification
   - Location: funcs.mk and individual package .mk files

3. **Hash Mismatches**
   - Cause: Modified source files or build scripts
   - Solution: Verify package integrity and build script consistency

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
3. Clean build directories: `rm -rf depends/work/build/`
4. Preserve cached packages: `depends/built/` if valid
5. Restart from clean state

---

**Remember**: This is an enterprise build system for end users. It should work cleanly without hacks, workarounds, or development shortcuts. Every change should maintain the professional, clean architecture designed for production use.