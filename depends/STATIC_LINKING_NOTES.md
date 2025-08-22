# Critical Static Linking Configuration for Windows Builds

## Date: 2025-08-21
## Author: MicroGuy / Claude

## CRITICAL: These settings prevent DLL dependencies in goldcoin-qt.exe

### 1. Qt6 Static Build Configuration
- **Location**: `packages/qt6.mk`
- **Critical Flag**: `$(package)_config_opts += -static`
- **Build Type**: Must use static libraries only (BUILD_SHARED_LIBS=OFF)

### 2. Windows Cross-Compilation Settings
- **CMAKE_SYSTEM_NAME**: Windows
- **CMAKE_EXECUTABLE_SUFFIX**: .exe
- **Platform**: win32-g++
- **Cross Compiler**: x86_64-w64-mingw32-g++

### 3. BerkeleyDB Configuration
- **Location**: `packages/bdb.mk`
- **Version**: 18.1.40 (with Windows mutex patches)
- **Critical Flags**: 
  - `--disable-shared`
  - `--enable-cxx`
  - `--enable-mingw` (for mingw32 builds)

### 4. OpenSSL Static Configuration
- **Location**: `packages/openssl.mk`
- **Version**: 1.0.1k
- **Critical Flag**: `no-shared`

### 5. All Support Libraries
- libevent: Built as static
- zeromq: Built as static
- zlib: Built as static (system-zlib for Qt)
- protobuf: Built as static
- qrencode: Built as static

## Verification Commands

```bash
# Check that all libraries are static (.a files, no .dll)
find x86_64-w64-mingw32 -name "*.dll" 2>/dev/null
# Should return nothing

# Check Qt6 static libraries
ls -la work/build/x86_64-w64-mingw32/qt6/*/lib/*.a | head -20

# Verify final goldcoin-qt.exe has no DLL dependencies (run after build)
x86_64-w64-mingw32-objdump -p goldcoin-qt.exe | grep "DLL Name"
# Should only show Windows system DLLs, no Qt or other library DLLs
```

## Two-Stage Qt6 Build Process
1. **Stage 1**: Build native Qt tools (moc, rcc, uic) for Linux host
2. **Stage 2**: Cross-compile Qt6 for Windows using native tools

## DO NOT MODIFY WITHOUT TEAM REVIEW
Any changes to these static linking configurations must be reviewed to prevent regression to dynamic linking.