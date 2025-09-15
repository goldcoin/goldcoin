# Goldcoin Core Release Build System
# Single command interface for end users
# All commands run from repository root

.PHONY: help deps-linux deps-windows linux windows windows-qt status

# Auto-detect job count if not specified
JOBS ?= $(shell nproc 2>/dev/null || echo 4)

# Repository root (absolute path)
REPO_ROOT = $(abspath .)

# Internal build authorization flag
CMAKE_INTERNAL_FLAG = -DGOLDCOIN_INTERNAL_BUILD=ON

help:
	@echo "=== Goldcoin Core Release Build System ==="
	@echo ""
	@echo "Dependencies (build once):"
	@echo "  make deps-linux         Build Linux dependencies in ../depends"
	@echo "  make deps-windows       Build Windows dependencies in ../depends"
	@echo ""
	@echo "Binaries:"
	@echo "  make linux              Build Linux binaries (goldcoind, goldcoin-cli)"
	@echo "  make windows            Build Windows binaries (goldcoind.exe, goldcoin-cli.exe)"
	@echo "  make windows-qt         Build Windows GUI (goldcoin-qt.exe)"
	@echo ""
	@echo "Job Control:"
	@echo "  make linux JOBS=4       Use 4 parallel jobs (default: $(JOBS))"
	@echo ""
	@echo "Maintenance:"
	@echo "  make status             Show build status"
	@echo ""
	@echo "Note: Run all commands from the repository root directory"

status:
	@echo "=== Build Status ==="
	@echo "Working directory: $$(pwd)"
	@echo "Repository: $(REPO_ROOT)"
	@echo "Jobs configured: $(JOBS)"
	@echo ""
	@echo "Dependencies:"
	@if [ -d "$(REPO_ROOT)/depends/x86_64-pc-linux-gnu" ]; then echo "  ✓ Linux depends ready"; else echo "  ✗ Linux depends missing - run: make deps-linux"; fi
	@if [ -d "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share" ]; then echo "  ✓ Windows depends ready"; else echo "  ✗ Windows depends missing - run: make deps-windows"; fi
	@echo ""
	@echo "Build directories:"
	@if [ -d "build/linux" ]; then echo "  ✓ Linux build exists"; else echo "  ✗ Linux build missing"; fi
	@if [ -d "build/windows" ]; then echo "  ✓ Windows build exists"; else echo "  ✗ Windows build missing"; fi
	@if [ -d "build/windows-qt" ]; then echo "  ✓ Windows Qt build exists"; else echo "  ✗ Windows Qt build missing"; fi

deps-linux:
	@echo "=== Building Linux Dependencies ==="
	@echo "Building in $(REPO_ROOT)/depends with $(JOBS) jobs..."
	$(MAKE) -C $(REPO_ROOT)/depends HOST=x86_64-pc-linux-gnu -j$(JOBS)
	@echo "✓ Linux dependencies complete"

deps-windows:
	@echo "=== Building Windows Dependencies ==="
	@echo "Building in $(REPO_ROOT)/depends with $(JOBS) jobs..."
	@echo "Note: This will build Qt 6.9 and may take time..."
	$(MAKE) -C $(REPO_ROOT)/depends HOST=x86_64-w64-mingw32 -j$(JOBS)
	@echo "✓ Windows dependencies complete"

deps-windows-no-qt:
	@echo "=== Building Windows Dependencies (No Qt) ==="
	@echo "Building in $(REPO_ROOT)/depends with $(JOBS) jobs..."
	$(MAKE) -C $(REPO_ROOT)/depends HOST=x86_64-w64-mingw32 NO_QT=1 -j$(JOBS)
	@echo "✓ Windows dependencies complete (no Qt)"

linux:
	@echo "=== Building Linux Binaries (Clean Build) ==="
	@if [ ! -d "$(REPO_ROOT)/depends/x86_64-pc-linux-gnu" ]; then \
		echo "❌ Linux dependencies missing. Run: make deps-linux"; \
		exit 1; \
	fi
	@if [ -d build/linux ]; then rm -rf build/linux; fi
	@mkdir -p build/linux
	cd build/linux && cmake $(REPO_ROOT) $(CMAKE_INTERNAL_FLAG) \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_GUI=OFF \
		-DBUILD_WALLET=ON \
		-DBUILD_STATIC=ON \
		-DUSE_DEPENDS=ON
	@echo "Starting build with $(JOBS) jobs..."
	$(MAKE) -C build/linux -j$(JOBS)
	@echo "✓ Linux build complete: build/linux/bin/"

windows:
	@echo "=== Building Windows Binaries (Clean Build) ==="
	@if [ ! -d "$(REPO_ROOT)/depends/x86_64-w64-mingw32" ]; then \
		echo "❌ Windows dependencies missing. Run: make deps-windows-no-qt"; \
		exit 1; \
	fi
	@if [ ! -f "$(REPO_ROOT)/depends/x86_64-w64-mingw32/lib/libcrypto.a" ]; then \
		echo "❌ OpenSSL libraries missing. Run: make deps-windows-no-qt"; \
		exit 1; \
	fi
	@if [ -d build/windows ]; then rm -rf build/windows; fi
	@if [ ! -f "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake" ]; then \
		echo "Creating Windows toolchain.cmake"; \
		mkdir -p "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share"; \
		echo 'set(CMAKE_SYSTEM_NAME Windows)' > "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo 'set(CMAKE_SYSTEM_PROCESSOR x86_64)' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo '' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo '# Explicit compiler settings for cross-compilation' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo 'set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc-posix)' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo 'set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++-posix)' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo 'set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo 'set(CMAKE_AR x86_64-w64-mingw32-gcc-ar-posix)' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo 'set(CMAKE_RANLIB x86_64-w64-mingw32-gcc-ranlib-posix)' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo '' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo '# Ensure cross-compilation is detected' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo 'set(CMAKE_CROSSCOMPILING TRUE)' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo '' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo '# Use depends-built libraries instead of system libraries' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo 'set(CMAKE_PREFIX_PATH "$(REPO_ROOT)/depends/x86_64-w64-mingw32")' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo 'set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo '' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo 'set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo 'set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo 'set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo '' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo '# Windows-specific definitions' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo 'add_definitions(-DWIN32)' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
	fi
	@echo "✓ Dependencies verified, proceeding with Windows build"
	@mkdir -p build/windows
	cd build/windows && cmake $(REPO_ROOT) $(CMAKE_INTERNAL_FLAG) \
		-DCMAKE_TOOLCHAIN_FILE=$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_GUI=OFF \
		-DBUILD_WALLET=ON \
		-DUSE_DEPENDS=ON
	@echo "Starting build with $(JOBS) jobs..."
	$(MAKE) -C build/windows -j$(JOBS)
	@echo "✓ Windows build complete: build/windows/bin/"

windows-qt:
	@echo "=== Building Windows GUI (Clean Build) ==="
	@if [ ! -d "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share" ]; then \
		echo "❌ Windows dependencies missing. Run: make deps-windows"; \
		exit 1; \
	fi
	@if [ -d build/windows-qt ]; then rm -rf build/windows-qt; fi
	@mkdir -p build/windows-qt
	cd build/windows-qt && cmake $(REPO_ROOT) $(CMAKE_INTERNAL_FLAG) \
		-DCMAKE_TOOLCHAIN_FILE=$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_GUI=ON \
		-DBUILD_WALLET=ON \
		-DUSE_DEPENDS=ON
	@echo "Starting build with $(JOBS) jobs..."
	$(MAKE) -C build/windows-qt -j$(JOBS)
	@echo "✓ Windows Qt build complete: build/windows-qt/bin/"


.DEFAULT_GOAL := help