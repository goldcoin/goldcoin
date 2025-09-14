# Goldcoin Core Enterprise Build System

Simple interface for building Goldcoin Core across platforms without breaking your development environment.

## Quick Start

```bash
# Show build options and status
make help
make status

# Build Linux binaries (goldcoind, goldcoin-cli)
make linux

# Build Windows binaries (goldcoind.exe, goldcoin-cli.exe)
make windows

# Build Windows GUI (goldcoin-qt.exe)
make windows-qt
```

## Key Features

- **Preserves existing builds** - Won't trigger depends rebuilds
- **Separate build directories** - Linux/Windows builds don't conflict
- **Smart job control** - Auto-detects CPU cores, allows override
- **Path detection** - Automatically finds Qt libraries for Windows GUI
- **Enterprise ready** - Consistent, reproducible builds

## Prerequisites

### Linux Builds
- Your existing `depends/x86_64-pc-linux-gnu/` (already built)
- GCC 15.2 (from depends or system)

### Windows Builds
- `depends/x86_64-w64-mingw32/` built with `make -C depends HOST=x86_64-w64-mingw32`
- For GUI: Qt 6.9 Windows libraries (see Qt Setup below)

## Build Commands

### Standard Usage
```bash
make linux              # Auto-detect CPU cores
make windows             # Cross-compile Windows binaries
make windows-qt          # Cross-compile Windows GUI
```

### Job Control
```bash
make linux JOBS=4       # Use 4 parallel jobs
make windows JOBS=8      # Use 8 parallel jobs (for powerful machines)
```

### Maintenance
```bash
make clean-linux         # Clean Linux builds only
make clean-windows       # Clean Windows builds only
make clean-all          # Clean all builds (preserves depends/)
make status             # Show current status and paths
```

## Qt Setup for Windows GUI

The Windows GUI build requires Qt 6.9 bundled libraries. The build system will:

1. **Auto-detect** Qt libraries in `/home/*/qt6-win-build/lib/`
2. **Allow override** via environment variable:
   ```bash
   QT_WIN_LIB_PATH=/path/to/qt6-win-build/lib make windows-qt
   ```

## Output Locations

All binaries are placed in separate build directories:

- **Linux**: `build/linux/bin/` - goldcoind, goldcoin-cli
- **Windows**: `build/windows/bin/` - goldcoind.exe, goldcoin-cli.exe  
- **Windows GUI**: `build/windows-qt/bin/` - goldcoin-qt.exe

## What's Preserved

This build system carefully preserves:
- ✅ Your existing `depends/` directories (no rebuilds)
- ✅ Your current development environment
- ✅ Ability to use traditional CMake commands
- ✅ All existing CMake functionality

## Traditional CMake Still Works

You can still use CMake directly for advanced configurations:

```bash
mkdir -p build/custom
cd build/custom
cmake ../.. [your options]
make
```

## Troubleshooting

### "Windows depends not found"
```bash
make -C depends HOST=x86_64-w64-mingw32
```

### "Qt Windows libraries not found"
Set the path explicitly:
```bash
QT_WIN_LIB_PATH=/your/path/qt6-win-build/lib make windows-qt
```

### Check build status
```bash
make status
```

## Architecture

- **Top-level Makefile** provides simple interface
- **CMakeLists.txt** handles complex build logic (unchanged)
- **Separate build dirs** prevent conflicts
- **Smart detection** finds paths automatically
- **Surgical changes** preserve existing functionality

The Qt atomic function pointer issue is resolved transparently through proper library path detection.

---

*This enterprise build system maintains full compatibility with your existing development workflow while providing a clean, simple interface for production builds.*