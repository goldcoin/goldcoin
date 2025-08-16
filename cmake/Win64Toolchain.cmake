# CMake Toolchain file for cross-compiling to Windows x64
# This is the MODERN way to build for Windows from Linux

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Specify the cross-compiler
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# Ensure we use POSIX threads (required for std::thread)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc-posix)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++-posix)

# Where to find dependencies
set(CMAKE_FIND_ROOT_PATH /home/microguy/git/microguy/goldcoin/depends/x86_64-w64-mingw32)

# Search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# Search for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Windows-specific settings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -static-libgcc -static-libstdc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -Wl,--no-undefined")

# Define Windows
add_definitions(-DWIN32 -D_WINDOWS -D_WIN32_WINNT=0x0601)