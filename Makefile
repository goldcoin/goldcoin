# Goldcoin Core Enterprise Build System
# Provides simple interface for complex CMake builds
# Usage: make linux, make windows, make windows-qt

.PHONY: help linux windows windows-qt clean-linux clean-windows clean-all status

# Auto-detect job count if not specified
JOBS ?= $(shell nproc 2>/dev/null || echo 4)

# Qt bundled lib path for Windows builds (auto-detect or override)
QT_WIN_LIB_PATH ?= $(shell find /home -name "qt6-win-build" -type d 2>/dev/null | head -1 | sed 's|$$|/lib|')

help:
	@echo "=== Goldcoin Core Enterprise Build System ==="
	@echo ""
	@echo "Simple Commands:"
	@echo "  make linux              Build native Linux binaries (goldcoind, goldcoin-cli)"
	@echo "  make windows            Build Windows binaries (goldcoind.exe, goldcoin-cli.exe)" 
	@echo "  make windows-qt         Build Windows GUI (goldcoin-qt.exe)"
	@echo ""
	@echo "Job Control:"
	@echo "  make linux JOBS=4       Use 4 parallel jobs"
	@echo "  make linux JOBS=8       Use 8 parallel jobs (default: auto-detect)"
	@echo ""
	@echo "Maintenance:"
	@echo "  make clean-linux        Clean Linux build artifacts"
	@echo "  make clean-windows      Clean Windows build artifacts" 
	@echo "  make clean-all          Clean all build artifacts"
	@echo "  make status             Show build status and paths"
	@echo ""
	@echo "Note: This preserves your existing depends/ - no rebuilds needed"

status:
	@echo "=== Build System Status ==="
	@echo "Current branch: $$(git branch --show-current 2>/dev/null || echo 'unknown')"
	@echo "Jobs configured: $(JOBS)"
	@echo "Qt Windows lib path: $(if $(QT_WIN_LIB_PATH),$(QT_WIN_LIB_PATH),not found)"
	@echo ""
	@echo "Build directories:"
	@if [ -d build/linux ]; then echo "  ✓ build/linux exists"; else echo "  ✗ build/linux not found"; fi
	@if [ -d build/windows ]; then echo "  ✓ build/windows exists"; else echo "  ✗ build/windows not found"; fi
	@if [ -d build/windows-qt ]; then echo "  ✓ build/windows-qt exists"; else echo "  ✗ build/windows-qt not found"; fi
	@echo ""
	@echo "Dependencies:"
	@if [ -d depends/x86_64-pc-linux-gnu ]; then echo "  ✓ Linux depends ready"; else echo "  ✗ Linux depends missing"; fi
	@if [ -d depends/x86_64-w64-mingw32 ]; then echo "  ✓ Windows depends ready"; else echo "  ✗ Windows depends missing"; fi

linux:
	@echo "=== Building Linux binaries (preserving existing setup) ==="
	@mkdir -p build/linux
	cd build/linux && cmake ../.. \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_GUI=OFF \
		-DBUILD_WALLET=ON \
		-DBUILD_STATIC=ON \
		-DUSE_DEPENDS=ON
	@echo "Starting parallel build with $(JOBS) jobs..."
	$(MAKE) -C build/linux -j$(JOBS)
	@echo "✓ Linux build complete: build/linux/bin/"

windows:
	@echo "=== Building Windows binaries ==="
	@if [ ! -d depends/x86_64-w64-mingw32 ]; then \
		echo "❌ Windows depends not found. Run: make -C depends HOST=x86_64-w64-mingw32"; \
		exit 1; \
	fi
	@mkdir -p build/windows
	cd build/windows && cmake ../.. \
		-DCMAKE_TOOLCHAIN_FILE=../../depends/x86_64-w64-mingw32/share/toolchain.cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_GUI=OFF \
		-DBUILD_WALLET=ON \
		-DUSE_DEPENDS=ON
	@echo "Starting parallel build with $(JOBS) jobs..."
	$(MAKE) -C build/windows -j$(JOBS)
	@echo "✓ Windows build complete: build/windows/bin/"

windows-qt:
	@echo "=== Building Windows GUI ==="
	@if [ ! -d depends/x86_64-w64-mingw32 ]; then \
		echo "❌ Windows depends not found. Run: make -C depends HOST=x86_64-w64-mingw32"; \
		exit 1; \
	fi
	@if [ -z "$(QT_WIN_LIB_PATH)" ] || [ ! -d "$(QT_WIN_LIB_PATH)" ]; then \
		echo "❌ Qt Windows libraries not found."; \
		echo "Please set QT_WIN_LIB_PATH=/path/to/qt6-win-build/lib"; \
		exit 1; \
	fi
	@mkdir -p build/windows-qt
	cd build/windows-qt && cmake ../.. \
		-DCMAKE_TOOLCHAIN_FILE=../../depends/x86_64-w64-mingw32/share/toolchain.cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_GUI=ON \
		-DBUILD_WALLET=ON \
		-DUSE_DEPENDS=ON \
		-DQT_BUNDLED_LIB_DIR="$(QT_WIN_LIB_PATH)"
	@echo "Starting parallel build with $(JOBS) jobs..."
	$(MAKE) -C build/windows-qt -j$(JOBS)
	@echo "✓ Windows Qt build complete: build/windows-qt/bin/"

clean-linux:
	@echo "Cleaning Linux build artifacts..."
	rm -rf build/linux
	@echo "✓ Linux build cleaned"

clean-windows:
	@echo "Cleaning Windows build artifacts..."
	rm -rf build/windows build/windows-qt
	@echo "✓ Windows build cleaned"

clean-all:
	@echo "Cleaning all build artifacts (preserving depends/)..."
	rm -rf build/
	@echo "✓ All builds cleaned (depends/ preserved)"

# Make help the default target
.DEFAULT_GOAL := help