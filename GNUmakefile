# Goldcoin Core Release Build System
# Single command interface for end users
# All commands run from repository root

.PHONY: help deps-linux deps-windows deps-i686 deps-aarch64 deps-arm linux i686 aarch64 arm windows windows-qt windows-installer status

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
	@echo "  make deps-i686          Build i686 (32-bit Linux) dependencies in ../depends"
	@echo "  make deps-aarch64       Build aarch64 (ARM64 Linux) dependencies in ../depends"
	@echo "  make deps-arm           Build arm (ARM32 Linux) dependencies in ../depends"
	@echo "  make deps-windows       Build Windows dependencies in ../depends"
	@echo ""
	@echo "Binaries:"
	@echo "  make linux              Build Linux binaries (goldcoind, goldcoin-cli)"
	@echo "  make i686               Build 32-bit Linux binaries (goldcoind, goldcoin-cli)"
	@echo "  make aarch64            Build ARM64 Linux binaries (goldcoind, goldcoin-cli)"
	@echo "  make arm                Build ARM32 Linux binaries (goldcoind, goldcoin-cli)"
	@echo "  make windows            Build Windows binaries (goldcoind.exe, goldcoin-cli.exe)"
	@echo "  make windows-qt         Build Windows GUI (goldcoin-qt.exe)"
	@echo "  make windows-installer  Package Windows GUI build into a setup.exe installer"
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
	@if [ -d "$(REPO_ROOT)/depends/i686-pc-linux-gnu" ]; then echo "  ✓ i686 depends ready"; else echo "  ✗ i686 depends missing - run: make deps-i686"; fi
	@if [ -d "$(REPO_ROOT)/depends/aarch64-linux-gnu" ]; then echo "  ✓ aarch64 depends ready"; else echo "  ✗ aarch64 depends missing - run: make deps-aarch64"; fi
	@if [ -d "$(REPO_ROOT)/depends/arm-linux-gnueabihf" ]; then echo "  ✓ arm depends ready"; else echo "  ✗ arm depends missing - run: make deps-arm"; fi
	@if [ -d "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share" ]; then echo "  ✓ Windows depends ready"; else echo "  ✗ Windows depends missing - run: make deps-windows"; fi
	@echo ""
	@echo "Build directories:"
	@if [ -d "build/linux" ]; then echo "  ✓ Linux build exists"; else echo "  ✗ Linux build missing"; fi
	@if [ -d "build/i686" ]; then echo "  ✓ i686 build exists"; else echo "  ✗ i686 build missing"; fi
	@if [ -d "build/aarch64" ]; then echo "  ✓ aarch64 build exists"; else echo "  ✗ aarch64 build missing"; fi
	@if [ -d "build/arm" ]; then echo "  ✓ arm build exists"; else echo "  ✗ arm build missing"; fi
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

deps-i686:
	@echo "=== Building i686 (32-bit Linux) Dependencies ==="
	@if ! command -v gcc-13 >/dev/null 2>&1 || ! command -v g++-13 >/dev/null 2>&1; then \
		echo "❌ gcc-13/g++-13 not found. Install with: sudo apt install -y g++-13"; \
		exit 1; \
	fi
	@if [ ! -f /usr/include/i386-linux-gnu/asm/errno.h ]; then \
		echo "❌ 32-bit kernel headers missing. Install with:"; \
		echo "   sudo dpkg --add-architecture i386 && sudo apt update && sudo apt install -y linux-libc-dev:i386 libc6-dev-i386 g++-multilib"; \
		exit 1; \
	fi
	@echo "Building in $(REPO_ROOT)/depends with $(JOBS) jobs..."
	$(MAKE) -C $(REPO_ROOT)/depends HOST=i686-pc-linux-gnu NO_QT=1 \
		i686_linux_CC="gcc-13 -m32" i686_linux_CXX="g++-13 -m32" -j$(JOBS)
	@echo "✓ i686 dependencies complete"

deps-aarch64:
	@echo "=== Building aarch64 (ARM64 Linux) Dependencies ==="
	@if ! command -v aarch64-linux-gnu-gcc >/dev/null 2>&1 || ! command -v aarch64-linux-gnu-g++ >/dev/null 2>&1; then \
		echo "❌ aarch64 cross-compiler not found. Install with:"; \
		echo "   sudo apt install -y gcc-aarch64-linux-gnu g++-aarch64-linux-gnu binutils-aarch64-linux-gnu"; \
		exit 1; \
	fi
	@echo "Building in $(REPO_ROOT)/depends with $(JOBS) jobs..."
	$(MAKE) -C $(REPO_ROOT)/depends HOST=aarch64-linux-gnu NO_QT=1 -j$(JOBS)
	@echo "✓ aarch64 dependencies complete"

deps-arm:
	@echo "=== Building arm (ARM32 Linux) Dependencies ==="
	@if ! command -v arm-linux-gnueabihf-gcc >/dev/null 2>&1 || ! command -v arm-linux-gnueabihf-g++ >/dev/null 2>&1; then \
		echo "❌ arm cross-compiler not found. Install with:"; \
		echo "   sudo apt install -y gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf binutils-arm-linux-gnueabihf"; \
		exit 1; \
	fi
	@echo "Building in $(REPO_ROOT)/depends with $(JOBS) jobs..."
	$(MAKE) -C $(REPO_ROOT)/depends HOST=arm-linux-gnueabihf NO_QT=1 -j$(JOBS)
	@echo "✓ arm dependencies complete"

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

i686:
	@echo "=== Building i686 (32-bit Linux) Binaries (Clean Build) ==="
	@if [ ! -d "$(REPO_ROOT)/depends/i686-pc-linux-gnu" ]; then \
		echo "❌ i686 dependencies missing. Run: make deps-i686"; \
		exit 1; \
	fi
	@if [ ! -f "$(REPO_ROOT)/depends/i686-pc-linux-gnu/share/toolchain.cmake" ]; then \
		echo "Creating i686 toolchain.cmake"; \
		mkdir -p "$(REPO_ROOT)/depends/i686-pc-linux-gnu/share"; \
		echo 'set(CMAKE_SYSTEM_PROCESSOR i686)' > "$(REPO_ROOT)/depends/i686-pc-linux-gnu/share/toolchain.cmake"; \
		echo 'set(CMAKE_CROSSCOMPILING FALSE)' >> "$(REPO_ROOT)/depends/i686-pc-linux-gnu/share/toolchain.cmake"; \
		echo 'set(GOLDCOIN_TARGET_ARCH "i686" CACHE STRING "Goldcoin target arch" FORCE)' >> "$(REPO_ROOT)/depends/i686-pc-linux-gnu/share/toolchain.cmake"; \
		echo 'set(CMAKE_C_COMPILER gcc-13)' >> "$(REPO_ROOT)/depends/i686-pc-linux-gnu/share/toolchain.cmake"; \
		echo 'set(CMAKE_CXX_COMPILER g++-13)' >> "$(REPO_ROOT)/depends/i686-pc-linux-gnu/share/toolchain.cmake"; \
		echo 'set(CMAKE_C_FLAGS "-m32 -msse2" CACHE STRING "C flags" FORCE)' >> "$(REPO_ROOT)/depends/i686-pc-linux-gnu/share/toolchain.cmake"; \
		echo 'set(CMAKE_CXX_FLAGS "-m32 -msse2" CACHE STRING "CXX flags" FORCE)' >> "$(REPO_ROOT)/depends/i686-pc-linux-gnu/share/toolchain.cmake"; \
		echo 'set(CMAKE_EXE_LINKER_FLAGS "-m32" CACHE STRING "Linker flags" FORCE)' >> "$(REPO_ROOT)/depends/i686-pc-linux-gnu/share/toolchain.cmake"; \
	fi
	@if [ -d build/i686 ]; then rm -rf build/i686; fi
	@mkdir -p build/i686
	cd build/i686 && cmake $(REPO_ROOT) $(CMAKE_INTERNAL_FLAG) \
		-DCMAKE_TOOLCHAIN_FILE=$(REPO_ROOT)/depends/i686-pc-linux-gnu/share/toolchain.cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_GUI=OFF \
		-DBUILD_WALLET=ON \
		-DUSE_DEPENDS=ON
	@echo "Starting build with $(JOBS) jobs..."
	$(MAKE) -C build/i686 -j$(JOBS)
	@echo "✓ i686 build complete: build/i686/bin/"

aarch64:
	@echo "=== Building aarch64 (ARM64 Linux) Binaries (Clean Build) ==="
	@if [ ! -d "$(REPO_ROOT)/depends/aarch64-linux-gnu" ]; then \
		echo "❌ aarch64 dependencies missing. Run: make deps-aarch64"; \
		exit 1; \
	fi
	@if [ ! -f "$(REPO_ROOT)/depends/aarch64-linux-gnu/share/toolchain.cmake" ]; then \
		echo "Creating aarch64 toolchain.cmake"; \
		mkdir -p "$(REPO_ROOT)/depends/aarch64-linux-gnu/share"; \
		echo 'set(CMAKE_SYSTEM_NAME Linux)' > "$(REPO_ROOT)/depends/aarch64-linux-gnu/share/toolchain.cmake"; \
		echo 'set(CMAKE_SYSTEM_PROCESSOR aarch64)' >> "$(REPO_ROOT)/depends/aarch64-linux-gnu/share/toolchain.cmake"; \
		echo 'set(CMAKE_CROSSCOMPILING TRUE)' >> "$(REPO_ROOT)/depends/aarch64-linux-gnu/share/toolchain.cmake"; \
		echo 'set(GOLDCOIN_TARGET_ARCH "aarch64" CACHE STRING "Goldcoin target arch" FORCE)' >> "$(REPO_ROOT)/depends/aarch64-linux-gnu/share/toolchain.cmake"; \
		echo 'set(GOLDCOIN_DEPENDS_TRIPLE "aarch64-linux-gnu" CACHE STRING "Goldcoin depends triple" FORCE)' >> "$(REPO_ROOT)/depends/aarch64-linux-gnu/share/toolchain.cmake"; \
		echo 'set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)' >> "$(REPO_ROOT)/depends/aarch64-linux-gnu/share/toolchain.cmake"; \
		echo 'set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)' >> "$(REPO_ROOT)/depends/aarch64-linux-gnu/share/toolchain.cmake"; \
		echo 'set(CMAKE_AR aarch64-linux-gnu-ar CACHE FILEPATH "Archiver")' >> "$(REPO_ROOT)/depends/aarch64-linux-gnu/share/toolchain.cmake"; \
		echo 'set(CMAKE_RANLIB aarch64-linux-gnu-ranlib)' >> "$(REPO_ROOT)/depends/aarch64-linux-gnu/share/toolchain.cmake"; \
		echo 'set(CMAKE_FIND_ROOT_PATH "$(REPO_ROOT)/depends/aarch64-linux-gnu")' >> "$(REPO_ROOT)/depends/aarch64-linux-gnu/share/toolchain.cmake"; \
		echo 'set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)' >> "$(REPO_ROOT)/depends/aarch64-linux-gnu/share/toolchain.cmake"; \
		echo 'set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)' >> "$(REPO_ROOT)/depends/aarch64-linux-gnu/share/toolchain.cmake"; \
		echo 'set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)' >> "$(REPO_ROOT)/depends/aarch64-linux-gnu/share/toolchain.cmake"; \
	fi
	@if [ -d build/aarch64 ]; then rm -rf build/aarch64; fi
	@mkdir -p build/aarch64
	cd build/aarch64 && cmake $(REPO_ROOT) $(CMAKE_INTERNAL_FLAG) \
		-DCMAKE_TOOLCHAIN_FILE=$(REPO_ROOT)/depends/aarch64-linux-gnu/share/toolchain.cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_GUI=OFF \
		-DBUILD_WALLET=ON \
		-DUSE_DEPENDS=ON
	@echo "Starting build with $(JOBS) jobs..."
	$(MAKE) -C build/aarch64 -j$(JOBS)
	@echo "✓ aarch64 build complete: build/aarch64/bin/"

arm:
	@echo "=== Building arm (ARM32 Linux) Binaries (Clean Build) ==="
	@if [ ! -d "$(REPO_ROOT)/depends/arm-linux-gnueabihf" ]; then \
		echo "❌ arm dependencies missing. Run: make deps-arm"; \
		exit 1; \
	fi
	@if [ ! -f "$(REPO_ROOT)/depends/arm-linux-gnueabihf/share/toolchain.cmake" ]; then \
		echo "Creating arm toolchain.cmake"; \
		mkdir -p "$(REPO_ROOT)/depends/arm-linux-gnueabihf/share"; \
		echo 'set(CMAKE_SYSTEM_NAME Linux)' > "$(REPO_ROOT)/depends/arm-linux-gnueabihf/share/toolchain.cmake"; \
		echo 'set(CMAKE_SYSTEM_PROCESSOR arm)' >> "$(REPO_ROOT)/depends/arm-linux-gnueabihf/share/toolchain.cmake"; \
		echo 'set(CMAKE_CROSSCOMPILING TRUE)' >> "$(REPO_ROOT)/depends/arm-linux-gnueabihf/share/toolchain.cmake"; \
		echo 'set(GOLDCOIN_TARGET_ARCH "arm" CACHE STRING "Goldcoin target arch" FORCE)' >> "$(REPO_ROOT)/depends/arm-linux-gnueabihf/share/toolchain.cmake"; \
		echo 'set(GOLDCOIN_DEPENDS_TRIPLE "arm-linux-gnueabihf" CACHE STRING "Goldcoin depends triple" FORCE)' >> "$(REPO_ROOT)/depends/arm-linux-gnueabihf/share/toolchain.cmake"; \
		echo 'set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)' >> "$(REPO_ROOT)/depends/arm-linux-gnueabihf/share/toolchain.cmake"; \
		echo 'set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)' >> "$(REPO_ROOT)/depends/arm-linux-gnueabihf/share/toolchain.cmake"; \
		echo 'set(CMAKE_C_FLAGS "-D_FILE_OFFSET_BITS=64" CACHE STRING "C flags" FORCE)' >> "$(REPO_ROOT)/depends/arm-linux-gnueabihf/share/toolchain.cmake"; \
		echo 'set(CMAKE_CXX_FLAGS "-D_FILE_OFFSET_BITS=64" CACHE STRING "CXX flags" FORCE)' >> "$(REPO_ROOT)/depends/arm-linux-gnueabihf/share/toolchain.cmake"; \
		echo 'set(CMAKE_AR arm-linux-gnueabihf-ar CACHE FILEPATH "Archiver")' >> "$(REPO_ROOT)/depends/arm-linux-gnueabihf/share/toolchain.cmake"; \
		echo 'set(CMAKE_RANLIB arm-linux-gnueabihf-ranlib)' >> "$(REPO_ROOT)/depends/arm-linux-gnueabihf/share/toolchain.cmake"; \
		echo 'set(CMAKE_FIND_ROOT_PATH "$(REPO_ROOT)/depends/arm-linux-gnueabihf")' >> "$(REPO_ROOT)/depends/arm-linux-gnueabihf/share/toolchain.cmake"; \
		echo 'set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)' >> "$(REPO_ROOT)/depends/arm-linux-gnueabihf/share/toolchain.cmake"; \
		echo 'set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)' >> "$(REPO_ROOT)/depends/arm-linux-gnueabihf/share/toolchain.cmake"; \
		echo 'set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)' >> "$(REPO_ROOT)/depends/arm-linux-gnueabihf/share/toolchain.cmake"; \
	fi
	@if [ -d build/arm ]; then rm -rf build/arm; fi
	@mkdir -p build/arm
	cd build/arm && cmake $(REPO_ROOT) $(CMAKE_INTERNAL_FLAG) \
		-DCMAKE_TOOLCHAIN_FILE=$(REPO_ROOT)/depends/arm-linux-gnueabihf/share/toolchain.cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_GUI=OFF \
		-DBUILD_WALLET=ON \
		-DUSE_DEPENDS=ON
	@echo "Starting build with $(JOBS) jobs..."
	$(MAKE) -C build/arm -j$(JOBS)
	@echo "✓ arm build complete: build/arm/bin/"

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
		echo 'set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc-win32)' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo 'set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++-win32)' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo 'set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo 'set(CMAKE_AR x86_64-w64-mingw32-gcc-ar-win32)' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
		echo 'set(CMAKE_RANLIB x86_64-w64-mingw32-gcc-ranlib-win32)' >> "$(REPO_ROOT)/depends/x86_64-w64-mingw32/share/toolchain.cmake"; \
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

windows-installer:
	@echo "=== Packaging Windows Installer ==="
	@if ! command -v makensis >/dev/null 2>&1; then \
		echo "❌ makensis not found. Install NSIS: sudo apt install nsis"; \
		exit 1; \
	fi
	@if [ ! -f "$(REPO_ROOT)/build/windows-qt/bin/goldcoin-qt.exe" ] || \
	    [ ! -f "$(REPO_ROOT)/build/windows-qt/bin/goldcoind.exe" ] || \
	    [ ! -f "$(REPO_ROOT)/build/windows-qt/bin/goldcoin-cli.exe" ]; then \
		echo "❌ Windows Qt build missing. Run: make windows-qt"; \
		exit 1; \
	fi
	@PACKAGE_NAME=$$(grep -m1 '^#define PACKAGE_NAME' src/version_info.h | sed 's/.*"\(.*\)"/\1/'); \
	PACKAGE_TARNAME=$$(grep -m1 '^#define PACKAGE_TARNAME' src/version_info.h | sed 's/.*"\(.*\)"/\1/'); \
	PACKAGE_URL=$$(grep -m1 '^#define PACKAGE_URL' src/version_info.h | sed 's/.*"\(.*\)"/\1/'); \
	VMAJOR=$$(grep -m1 'set(CLIENT_VERSION_MAJOR' CMakeLists.txt | grep -o '[0-9]\+'); \
	VMINOR=$$(grep -m1 'set(CLIENT_VERSION_MINOR' CMakeLists.txt | grep -o '[0-9]\+'); \
	VREV=$$(grep -m1 'set(CLIENT_VERSION_REVISION' CMakeLists.txt | grep -o '[0-9]\+'); \
	VBUILD=$$(grep -m1 'set(CLIENT_VERSION_BUILD' CMakeLists.txt | grep -o '[0-9]\+'); \
	sed \
		-e "s|@PACKAGE_NAME@|$$PACKAGE_NAME|g" \
		-e "s|@PACKAGE_TARNAME@|$$PACKAGE_TARNAME|g" \
		-e "s|@PACKAGE_URL@|$$PACKAGE_URL|g" \
		-e "s|@CLIENT_VERSION_MAJOR@|$$VMAJOR|g" \
		-e "s|@CLIENT_VERSION_MINOR@|$$VMINOR|g" \
		-e "s|@CLIENT_VERSION_REVISION@|$$VREV|g" \
		-e "s|@CLIENT_VERSION_BUILD@|$$VBUILD|g" \
		-e "s|@WINDOWS_BITS@|64|g" \
		-e "s|@EXEEXT@|.exe|g" \
		-e "s|@BITCOIN_GUI_NAME@|goldcoin-qt|g" \
		-e "s|@BITCOIN_DAEMON_NAME@|goldcoind|g" \
		-e "s|@BITCOIN_CLI_NAME@|goldcoin-cli|g" \
		-e "s|@abs_top_srcdir@|$(REPO_ROOT)|g" \
		-e "s|$(REPO_ROOT)/doc/README_windows.txt|$(REPO_ROOT)/README.md|g" \
		-e "s|$(REPO_ROOT)/release/|$(REPO_ROOT)/build/windows-qt/bin/|g" \
		share/setup.nsi.in > share/setup.nsi; \
	makensis share/setup.nsi
	@echo "✓ Windows installer complete: goldcoin-*-win64-setup.exe"

.DEFAULT_GOAL := help