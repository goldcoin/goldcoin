# Goldcoin v0.16.0 Release Notes

**Release Date:** July 30, 2025  
**Compatibility:** C++11 | Python 3.12+ | GCC 13+ | Windows 10/11 | Ubuntu 20.04+

---

## Overview

Goldcoin v0.16.0 is a major refactor release that modernizes the entire codebase from C++03 to C++11, ensures Python 3.12 compatibility, and eliminates legacy dependencies. This foundational update ensures Goldcoin remains buildable and functional on modern systems for years to come.

---

## 🎯 Foundational Change: C++11 Standard Adoption

### Upgraded from C++03 to C++11
This fundamental change enables all other modernization efforts in v0.16.0:

- **Mandatory C++11 Standard**: Added explicit `-std=c++11` flag (no GNU extensions)
- **Disabled Legacy Features**:
  - `BOOST_NO_CXX98_FUNCTION_BASE`
  - `BOOST_NO_CXX98_BINDERS`

### C++11 Features Now Utilized
- ✅ **Modern Threading**: `std::mutex`, `std::condition_variable`, `std::thread`
- ✅ **Initializer Lists**: Replaced `boost::assign::list_of`
- ✅ **Range-based For Loops**: Cleaner, safer iteration
- ✅ **Type Inference**: `auto` keyword usage throughout
- ✅ **Move Semantics**: Improved performance with rvalue references
- ✅ **Smart Pointers**: `std::unique_ptr`, `std::shared_ptr`
- ✅ **nullptr**: Type-safe null pointer constant
- ✅ **Standard Algorithms**: Replaced Boost equivalents

### Why C++11 Matters
1. **Enables Modern Dependencies**: Required for Boost 1.64+ compatibility
2. **Facilitates Migration**: Allowed replacement of 12 Boost modules
3. **Compiler Support**: Essential for GCC 13+, Clang 15+
4. **Performance**: Better optimization opportunities
5. **Future-Proofing**: Foundation for eventual C++14/17 adoption

---

## Major Features

### 🐍 Python 3.12 Compatibility
- Full support for Python 3.12's strict `PY_SSIZE_T_CLEAN` requirement
- Native crypto bindings (faster than pip packages)
- Maintains compatibility with Python 3.8+
- Essential for systems beyond 2024

### 🔧 Boost Library Modernization
Replaced 12 deprecated Boost modules with C++11 standard library:
- `boost::algorithm::split` → `std::stringstream`
- `boost::algorithm::replace_all` → `std::string` methods
- `boost::assign::list_of` → C++11 initializer lists
- `boost::posix_time` → `<chrono>` and `<ctime>`
- `boost::foreach` → range-based for loops
- `boost::function` → `std::function`
- `boost::bind` → `std::bind` and lambdas
- And 5 more modules eliminated

### 📦 Enhanced Windows Distribution
- **NEW**: Portable ZIP archive option
  - `goldcoin-0.16.0-win64.zip`
  - No installation required
  - Perfect for USB drives
- Traditional installer maintained
  - `goldcoin-0.16.0-win64-setup.exe`
  - Full Windows integration

### 🚀 Complete CI/CD Overhaul
- Migrated from Travis CI to GitHub Actions
- Automated builds for all supported platforms
- Self-hosted runner support
- Parallel build matrix
- Artifact generation and upload

---

## Bug Fixes (151 commits)

### Build System Fixes
- Fixed missing headers: `<mutex>`, `<condition_variable>`, `<thread>`, `<stdexcept>`, `<algorithm>`
- Resolved Berkeley DB cross-compilation issues
- Fixed endian.h compatibility using compiler intrinsics
- Corrected malloc.h vs stdlib.h for macOS
- Fixed Qt5 detection and configuration
- Resolved i686 32-bit builds using native containers

### Test Suite Improvements
- Fixed addrman_tests deterministic address selection
- Fixed miner_tests CreateNewBlock exception handling
- Fixed rpc_tests ban tests for CI environment
- Fixed transaction_tests script validation
- Fixed duplicate txin detection
- Fixed RPC JSON validation
- Improved test reliability in parallel environments

### Compiler Compatibility
- Fixed GCC 13+ concept check failures
- Resolved debug iterator compatibility (MSVC)
- Fixed C++11 compliance throughout
- Eliminated cast-function-type warnings
- Fixed std::unary_function deprecation

---

## Platform Support

| Platform | Architecture | Status | Compiler | Package |
|----------|-------------|---------|----------|---------|
| **Linux** | x86_64 | ✅ Full Support | GCC 9+ | `.tar.gz` |
| **Linux** | i686 | ✅ Full Support | GCC 9+ | `.tar.gz` |
| **Linux** | ARM64/aarch64 | ✅ Full Support | GCC 9+ | `.tar.gz` |
| **Windows** | x86_64 | ✅ Full Support | MinGW-w64 | `.exe` + `.zip` |
| **macOS** | x86_64/ARM64 | ⏳ Manual builds only | Clang 12+ | - |

### Minimum Requirements
- **C++ Standard**: C++11 (mandatory)
- **Python**: 3.8+ (3.12 recommended)
- **Compiler**: GCC 9+, Clang 10+, MSVC 2019+
- **Boost**: 1.64+ (partially eliminated)

---

## Technical Details

### Dependency Updates
- **C++ Standard**: C++03 → C++11 ✅
- **Python**: 3.11 → 3.12 ✅
- **Boost**: Extensive modernization (12 modules removed)
- **Build System**: Autotools updates for modern systems
- **CI/CD**: Travis CI → GitHub Actions ✅

### Performance Improvements
- Native Python crypto extension
- Move semantics throughout codebase
- Reduced Boost overhead
- Parallel CI builds (70% faster)
- Compiler optimization improvements

### Code Quality
- Eliminated ~1,000 lines of legacy code
- Fixed all deprecation warnings
- Improved const-correctness
- Enhanced exception handling
- Modern C++ best practices

---

## Installation Instructions

### Windows Users

**Option 1: Traditional Installer** - `goldcoin-0.16.0-win64-setup.exe`
- Automated installation with setup wizard
- Start Menu and Desktop shortcuts
- Includes uninstaller

**Option 2: Portable ZIP Archive (NEW)** - `goldcoin-0.16.0-win64.zip`
- Extract to any location and run
- No installation or registry changes
- Ideal for USB drives

### Linux Users
```bash
# Extract the archive
tar -xzf goldcoin-0.16.0-x86_64-linux-gnu.tar.gz

# Install to system (optional)
sudo install -m 755 goldcoin-*/bin/* /usr/local/bin/

# Or run directly from extracted directory
./goldcoin-0.16.0/bin/goldcoin-qt
