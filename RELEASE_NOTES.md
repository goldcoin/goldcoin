# Goldcoin v0.16.0 Release Notes

**Release Date:** July 30, 2025  
**Compatibility:** Python 3.12+ | GCC 13+ | Windows 10/11 | Ubuntu 20.04+

---

## Overview

Goldcoin v0.16.0 is a critical compatibility update that modernizes the codebase for current and future toolchains. This release ensures Goldcoin remains buildable and functional on modern systems with Python 3.12, GCC 13+, and the latest operating systems.

---

## Major Features

### 🐍 Python 3.12 Support
- Full compatibility with Python 3.12's strict `PY_SSIZE_T_CLEAN` requirement
- Updated all Python bindings and crypto interfaces
- Native support for Python 3.12's enhanced security features
- Maintains backward compatibility with Python 3.8+

### 🔧 Modernized Dependencies
- **Replaced 12 Boost modules** with standard C++11/14 equivalents:
  - `boost/algorithm/string/classification.hpp` → `<cctype>`
  - `boost/algorithm/string/replace.hpp` → `<algorithm>` + `<string>`
  - `boost/algorithm/string/split.hpp` → Custom implementation
  - And 9 more Boost dependencies eliminated
- Updated for GCC 13+ and Clang 15+ compatibility
- Implemented compiler-agnostic endian handling

### 📦 Enhanced Windows Distribution
- **NEW**: Portable ZIP archive option
  - No installation required
  - Run from USB or network drives
  - Ideal for advanced users
- Traditional installer still available
- Both options created with `make deploy`

### 🚀 CI/CD Infrastructure
- Complete GitHub Actions workflow
- Automated builds for all platforms
- Continuous integration testing
- Automated release preparation

---

## Platform Support

| Platform | Architecture | Status | Package Format |
|----------|-------------|---------|----------------|
| **Linux** | x86_64 | ✅ Full Support | `.tar.gz` |
| **Linux** | ARM64/aarch64 | ✅ Full Support | `.tar.gz` |
| **Windows** | x86_64 | ✅ Full Support | `.exe` installer + `.zip` |
| **macOS** | x86_64/ARM64 | ⏳ Deferred to v0.16.1 | - |

### Minimum Requirements
- **Linux**: Ubuntu 20.04 LTS or equivalent
- **Windows**: Windows 10 version 1809 or later
- **Compiler**: GCC 9+ or Clang 10+
- **Python**: 3.8 or later (3.12 recommended)

---

## Technical Improvements

### Build System
- Fixed C++17 standard library compatibility
- Resolved debug iterator issues with modern STL
- Updated autotools configuration for modern compilers
- Improved cross-compilation support

### Code Quality
- Eliminated deprecated function usage
- Fixed all compiler warnings on GCC 13
- Improved const-correctness throughout
- Enhanced error handling and reporting

### Windows Specific
- Updated NSIS installer to v0.16.0
- Added automatic ZIP archive generation
- Improved Qt5 deployment
- Fixed Windows-specific endian handling

---

### Windows Users

Choose your preferred installation method:

**Option 1: Traditional Installer** - `goldcoin-0.16.0-win64-setup.exe`
- Automated installation with setup wizard
- Start Menu and Desktop shortcuts
- Automatic file associations
- Includes uninstaller
- Recommended for most users

**Option 2: Portable ZIP Archive (NEW)** - `goldcoin-0.16.0-win64.zip`
- No installation required
- Extract to any location and run
- Ideal for USB drives or portable use
- No registry modifications
- Perfect for advanced users or testing

#### Installation Instructions

**For the Installer:**
1. Download the installer file (.exe)
2. Run as Administrator
3. Follow the setup wizard
4. Launch from Start Menu or Desktop shortcut

**For the Portable Archive:**
1. Download the ZIP file
2. Extract to your desired location
3. Navigate to the extracted folder
4. Run `goldcoin-qt.exe` to start the wallet

#### System Requirements
- Windows 10 version 1809 or later
- Windows 11 (all versions)
- 64-bit processor
- 4GB RAM minimum (8GB recommended)
- 20GB free disk space
