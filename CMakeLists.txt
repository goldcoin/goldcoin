cmake_minimum_required(VERSION 3.21)

# ===== SET COMPILER BEFORE PROJECT =====
# Only set compiler for native Linux builds, not when using a toolchain file
if(NOT CMAKE_TOOLCHAIN_FILE AND NOT DEFINED CMAKE_C_COMPILER)
    # Native Linux build - use GCC 15.2 for full C++23 support
    # Check for depends-built GCC first, fall back to system
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/depends/x86_64-pc-linux-gnu/native/bin/gcc")
        set(CMAKE_C_COMPILER "${CMAKE_CURRENT_SOURCE_DIR}/depends/x86_64-pc-linux-gnu/native/bin/gcc" CACHE STRING "C compiler" FORCE)
        set(CMAKE_CXX_COMPILER "${CMAKE_CURRENT_SOURCE_DIR}/depends/x86_64-pc-linux-gnu/native/bin/g++" CACHE STRING "C++ compiler" FORCE)
        message(STATUS "Using depends GCC 15.2")
    elseif(EXISTS "/usr/bin/gcc-15")
        set(CMAKE_C_COMPILER "/usr/bin/gcc-15" CACHE STRING "C compiler" FORCE)
        set(CMAKE_CXX_COMPILER "/usr/bin/g++-15" CACHE STRING "C++ compiler" FORCE)
    endif()
endif()

project(goldcoin VERSION 0.17.0 LANGUAGES CXX C ASM)

if(NOT GOLDCOIN_INTERNAL_BUILD)
      message(FATAL_ERROR "Do not call cmake directly! Use: make linux, make windows, or make windows-qt")
  endif()

# Force proper C++23 on MinGW (CMake 3.22 may not map it)
if(MINGW OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Honor the standard but also explicitly set the flag so CMake doesn't downshift to C++20
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS ON)  # we use gnu++23 typically
    
    # Explicitly add the flag in case this CMake version doesn't know C++23 for MinGW
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-std=gnu++23>)
    
    # Ensure all targets see secp256k1 as static library (prevents __imp_ symbols)
    add_compile_definitions(SECP256K1_STATIC)
endif()

# ===== BUILD OPTIONS =====
option(USE_DEPENDS "Use depends/ prefix for all dependencies" ON)
option(BUILD_GUI "Build goldcoin-qt GUI with Qt 6.9" OFF)
option(BUILD_WALLET "Enable wallet functionality" ON)
option(BUILD_TESTS "Build test suite" OFF)
option(BUILD_BENCH "Build benchmarks" OFF)
option(WITH_ZMQ "Enable ZMQ notifications" OFF)
option(WITH_MINIUPNPC "Enable UPnP support" OFF)
option(BUILD_STATIC "Build static binaries (Linux only)" ON)

# Default: C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON) # we use gnu++* on Linux/MinGW

# We have verified MinGW GCC 13+ supports C++23
# Removing fallback to C++20 since we have proper toolchain

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# C11 standard for C files
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# C++ specific flags - force C++23 for all builds
add_compile_options(
    $<$<COMPILE_LANGUAGE:CXX>:-std=c++23>
    $<$<COMPILE_LANGUAGE:CXX>:-fconcepts>
    $<$<COMPILE_LANGUAGE:CXX>:-fcoroutines>
)

# ===== PLATFORM FEATURE DETECTION =====
include(CheckFunctionExists)

# Platform-specific feature detection
if(NOT WIN32 AND NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Unix/Linux features
    check_function_exists(daemon HAVE_DECL_DAEMON)
    
    # Check for mallopt (glibc-specific)
    check_function_exists(mallopt HAVE_MALLOPT_ARENA_MAX)
    if(HAVE_MALLOPT_ARENA_MAX)
        add_compile_definitions(HAVE_MALLOPT_ARENA_MAX=1)
    endif()
    
    # Check for strnlen declaration
    check_function_exists(strnlen HAVE_DECL_STRNLEN)
    if(HAVE_DECL_STRNLEN)
        set(HAVE_DECL_STRNLEN 1)
    else()
        set(HAVE_DECL_STRNLEN 0)
    endif()
	else()
      # Windows doesn't have daemon or mallopt (glibc-specific), but has strnlen
      set(HAVE_DECL_DAEMON 0)
      set(HAVE_DECL_STRNLEN 1)
      # mallopt not available on Windows - skip HAVE_MALLOPT_ARENA_MAX
endif()

# ===== BUILD TYPE =====
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ===== CONFIG FILE GENERATION =====
# Generate bitcoin-config.h
set(CLIENT_VERSION_MAJOR 0)
set(CLIENT_VERSION_MINOR 17)
set(CLIENT_VERSION_REVISION 0)
set(CLIENT_VERSION_BUILD 0)
set(CLIENT_VERSION_IS_RELEASE true)
set(COPYRIGHT_HOLDERS "The Goldcoin Core developers")
set(COPYRIGHT_YEAR "2025")
set(PACKAGE_NAME "Goldcoin Core")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/src/config/bitcoin-config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/src/config/bitcoin-config.h"
    @ONLY
)

# secp256k1 v0.6.0 handles its own configuration via modern CMake

# ===== OUTPUT DIRECTORIES =====
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ===== BUILD CONFIGURATION =====
# For native Linux builds: Use Qt6 from depends but SYSTEM OpenSSL/libs
option(USE_DEPENDS_QT "Use Qt6 from depends (native builds)" OFF)

# GOLDCOIN_TARGET_ARCH is the bare arch name (x86_64, i686, aarch64, ...).
# CMake's own CMAKE_SYSTEM_PROCESSOR gets silently reset to the host's real
# arch whenever CMAKE_CROSSCOMPILING is FALSE, so it can't be trusted for
# same-OS/different-word-size targets like i686 - toolchain files set this
# cache variable instead, which CMake's internals never touch.
if(NOT DEFINED GOLDCOIN_TARGET_ARCH)
    set(GOLDCOIN_TARGET_ARCH "${CMAKE_SYSTEM_PROCESSOR}")
endif()

# GOLDCOIN_DEPENDS_TRIPLE is the depends/ subdirectory name. Most Linux hosts
# depends builds to <arch>-pc-linux-gnu, but Debian/Ubuntu's real multiarch
# triplets for ARM don't include "-pc-" (aarch64-linux-gnu,
# arm-linux-gnueabihf) - toolchain files for those override this directly
# rather than have it derived from GOLDCOIN_TARGET_ARCH.
if(NOT DEFINED GOLDCOIN_DEPENDS_TRIPLE)
    set(GOLDCOIN_DEPENDS_TRIPLE "${GOLDCOIN_TARGET_ARCH}-pc-linux-gnu")
endif()

# Platform-specific dependency detection
# NOTE: only match on CMAKE_SYSTEM_NAME, not merely "a toolchain file is set" -
# non-Windows cross/32-bit targets (i686, aarch64, arm-linux-gnueabihf, ...)
# also need a toolchain file and must not be routed into the Windows branch.
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Cross-compiling to Windows
    set(CROSS_COMPILING_TO_WINDOWS TRUE)
    set(DEPENDS_PREFIX "${CMAKE_SOURCE_DIR}/depends/x86_64-w64-mingw32")
    set(USE_DEPENDS ON)
    message(STATUS "Cross-compile mode - Windows target detected")
    message(STATUS "Using Windows depends: ${DEPENDS_PREFIX}")
    
    # Ensure Windows depends are used exclusively
    list(PREPEND CMAKE_PREFIX_PATH "${DEPENDS_PREFIX}")
    set(CMAKE_FIND_ROOT_PATH "${DEPENDS_PREFIX}")
    
    # Prevent mixing with Linux dependencies
    set(OPENSSL_ROOT_DIR "${DEPENDS_PREFIX}")
    set(OPENSSL_INCLUDE_DIR "${DEPENDS_PREFIX}/include")
    set(OPENSSL_SSL_LIBRARY "${DEPENDS_PREFIX}/lib64/libssl.a")
    set(OPENSSL_CRYPTO_LIBRARY "${DEPENDS_PREFIX}/lib64/libcrypto.a")
    
elseif(USE_DEPENDS)
    # Full depends mode for cross-compilation AND native static builds
    if(WIN32)
        set(DEPENDS_PREFIX "${CMAKE_SOURCE_DIR}/depends/x86_64-w64-mingw32")
        message(STATUS "Cross-compile mode - Using full depends: ${DEPENDS_PREFIX}")
    else()
        set(DEPENDS_PREFIX "${CMAKE_SOURCE_DIR}/depends/${GOLDCOIN_DEPENDS_TRIPLE}")
        message(STATUS "Native static build - Using full depends: ${DEPENDS_PREFIX}")
    endif()
    list(APPEND CMAKE_PREFIX_PATH "${DEPENDS_PREFIX}")
    set(CMAKE_FIND_ROOT_PATH "${DEPENDS_PREFIX}")
    
    # Force depends libraries over system libraries for enterprise reproducibility
    # Depends installs to lib64/ on 64-bit hosts but plain lib/ on 32-bit hosts (e.g. i686)
    set(OPENSSL_ROOT_DIR "${DEPENDS_PREFIX}")
    set(OPENSSL_USE_STATIC_LIBS ON)
    if(EXISTS "${DEPENDS_PREFIX}/lib64/libssl.a")
        set(OPENSSL_SSL_LIBRARY "${DEPENDS_PREFIX}/lib64/libssl.a")
        set(OPENSSL_CRYPTO_LIBRARY "${DEPENDS_PREFIX}/lib64/libcrypto.a")
    else()
        set(OPENSSL_SSL_LIBRARY "${DEPENDS_PREFIX}/lib/libssl.a")
        set(OPENSSL_CRYPTO_LIBRARY "${DEPENDS_PREFIX}/lib/libcrypto.a")
    endif()
    
    # For x86_64 Linux static builds, use system libevent (static .a files) for true static linking
    # This allows proper glibc static linking to achieve 22MB binary
    if(NOT WIN32 AND GOLDCOIN_TARGET_ARCH STREQUAL "x86_64")
        # Use pkg-config to find system libevent - linker will use .a files with -static flag
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(LIBEVENT REQUIRED libevent)
        pkg_check_modules(LIBEVENT_PTHREADS REQUIRED libevent_pthreads)
        # Don't override - let pkg-config and -static flag handle it
    else()
        # Windows, and non-x86_64 Linux hosts (e.g. i686), use depends libevent -
        # the system libevent is the wrong architecture for cross/32-bit builds
        if(EXISTS "${DEPENDS_PREFIX}/lib64/libevent.a")
            set(LIBEVENT_LIBRARIES "${DEPENDS_PREFIX}/lib64/libevent.a")
            set(LIBEVENT_PTHREADS_LIBRARIES "${DEPENDS_PREFIX}/lib64/libevent_pthreads.a")
        else()
            set(LIBEVENT_LIBRARIES "${DEPENDS_PREFIX}/lib/libevent.a")
            set(LIBEVENT_PTHREADS_LIBRARIES "${DEPENDS_PREFIX}/lib/libevent_pthreads.a")
        endif()
        set(LIBEVENT_INCLUDE_DIRS "${DEPENDS_PREFIX}/include")
    endif()
    
elseif(USE_DEPENDS_QT)
    # Native build with Qt6 from depends ONLY
    set(DEPENDS_PREFIX "${CMAKE_SOURCE_DIR}/depends/${GOLDCOIN_DEPENDS_TRIPLE}")
    # Set Qt6_DIR and also add to CMAKE_PREFIX_PATH for bundled libraries
    set(Qt6_DIR "${DEPENDS_PREFIX}/lib/cmake/Qt6")
    list(APPEND CMAKE_PREFIX_PATH "${DEPENDS_PREFIX}/lib/cmake")
    message(STATUS "Native build - Using Qt6 from depends ONLY")
    message(STATUS "Qt6_DIR: ${Qt6_DIR}")
    message(STATUS "DEPENDS_PREFIX: ${DEPENDS_PREFIX}")
else()
    # Native Linux build
    set(DEPENDS_PREFIX "${CMAKE_SOURCE_DIR}/depends/${GOLDCOIN_DEPENDS_TRIPLE}")
    message(STATUS "Native build - Using system Qt6 and libraries")
endif()

# ===== CORE DEPENDENCIES =====

# Threads
find_package(Threads REQUIRED)

# OpenSSL configuration
if(WIN32 OR USE_DEPENDS)
    # For Windows/cross-compile, use OpenSSL from depends
    set(OPENSSL_ROOT_DIR "${DEPENDS_PREFIX}")
    set(OPENSSL_USE_STATIC_LIBS ON)
    find_package(OpenSSL 1.0 REQUIRED)
else()
    # For native Linux builds, use modern system OpenSSL
    set(OPENSSL_ROOT_DIR "/usr")
    set(OPENSSL_USE_STATIC_LIBS OFF)
    # Explicitly exclude depends OpenSSL
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
    find_package(OpenSSL REQUIRED)
endif()
message(STATUS "OpenSSL version: ${OPENSSL_VERSION}")

# Version check - require modern OpenSSL 3.0+
if(NOT WIN32 AND NOT USE_DEPENDS)
    if(OPENSSL_VERSION VERSION_LESS "3.0.0")
        message(FATAL_ERROR "OpenSSL 3.0+ required but found ${OPENSSL_VERSION}")
    endif()
endif()

message(STATUS "Using modern OpenSSL ${OPENSSL_VERSION}")
message(STATUS "OpenSSL include: ${OPENSSL_INCLUDE_DIR}")
message(STATUS "OpenSSL SSL lib: ${OPENSSL_SSL_LIBRARY}")

# Reset find modes after OpenSSL
if(NOT USE_DEPENDS)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
endif()

# Boost
# Boost completely removed - using C++23 standard library
# All boost::thread replaced with std::thread
# find_package(Boost 1.47 REQUIRED COMPONENTS thread)

# Platform-specific dependencies
if(NOT WIN32)
    # For static builds, pkg-config sets LIBEVENT_LIBRARIES to -levent
    # With -static flag, linker will use /usr/lib/x86_64-linux-gnu/libevent.a
    
    # Other system libraries
    # BIP70 removed - protobuf no longer needed
    # pkg_check_modules(PROTOBUF REQUIRED protobuf)
    
    # Don't use system FreeType - causes version conflicts with HarfBuzz
    # We'll use the one from depends which is compatible
    
    set(PLATFORM_LIBS 
        ${LIBEVENT_LIBRARIES} 
        ${LIBEVENT_PTHREADS_LIBRARIES}
        # ${PROTOBUF_LIBRARIES}  # BIP70 removed 
        dl
    )
else()
    # Windows libraries
    # Add libevent from depends for Windows builds
    set(PLATFORM_LIBS 
        ${DEPENDS_PREFIX}/lib/libevent.a
        ${DEPENDS_PREFIX}/lib/libevent_core.a
        ws2_32 
        mswsock 
        shlwapi 
        iphlpapi
        crypt32
        winmm
        kernel32
        user32
        gdi32
        comdlg32
        shell32
        ole32
        oleaut32
        uuid
        advapi32
    )
endif()

# BerkeleyDB 18.1 (for wallet)
if(BUILD_WALLET)
    # Platform-specific BDB detection
    if(CROSS_COMPILING_TO_WINDOWS)
        # For Windows, use the Windows depends path directly
        set(BDB_PREFIX "${DEPENDS_PREFIX}")
    else()
        # For Linux, use the Linux depends path
        set(BDB_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/depends/${GOLDCOIN_DEPENDS_TRIPLE}")
    endif()
    
    # Check for the appropriate BDB version
    if(CROSS_COMPILING_TO_WINDOWS AND EXISTS "${BDB_PREFIX}/lib/libdb_cxx.a")
        # Windows build uses generic names
        set(BDB_CXX_LIBRARY "${BDB_PREFIX}/lib/libdb_cxx.a")
        set(BDB_LIBRARY "${BDB_PREFIX}/lib/libdb.a")
        set(BDB_INCLUDE_DIR "${BDB_PREFIX}/include")
        message(STATUS "Using BerkeleyDB from Windows depends: ${BDB_PREFIX}")
    elseif(EXISTS "${BDB_PREFIX}/lib/libdb_cxx-18.1.a")
        set(BDB_CXX_LIBRARY "${BDB_PREFIX}/lib/libdb_cxx-18.1.a")
        set(BDB_LIBRARY "${BDB_PREFIX}/lib/libdb-18.1.a")
        set(BDB_INCLUDE_DIR "${BDB_PREFIX}/include")
        message(STATUS "Using BerkeleyDB 18.1 from: ${BDB_PREFIX}")
    else()
        # Try to find system BDB as fallback
        find_library(BDB_CXX_LIBRARY NAMES db_cxx-18.1 db_cxx-5.3 db_cxx-4.8 db_cxx db5-cxx db-cxx 
                     PATHS /usr/lib/x86_64-linux-gnu /usr/lib)
        find_library(BDB_LIBRARY NAMES db-18.1 db-5.3 db-4.8 db 
                     PATHS /usr/lib/x86_64-linux-gnu /usr/lib)
        find_path(BDB_INCLUDE_DIR NAMES db_cxx.h 
                  PATHS /usr/include /usr/local/include /usr/include/db18 /usr/include/db5.3)
    endif()
    
    if(NOT BDB_CXX_LIBRARY OR NOT BDB_LIBRARY)
        message(FATAL_ERROR "Berkeley DB not found - required for wallet functionality")
    endif()
    
    set(BDB_LIBRARIES ${BDB_CXX_LIBRARY} ${BDB_LIBRARY})
    
    # Add BDB include directory to wallet compilation
    if(BDB_INCLUDE_DIR)
        include_directories(${BDB_INCLUDE_DIR})
    endif()
endif()

# ===== GLOBAL INCLUDES (MINIMAL) =====
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}/src  # Generated headers (for config/ subdirs)
    ${CMAKE_CURRENT_BINARY_DIR}/src/config  # Direct access to bitcoin-config.h
    # secp256k1 v0.6.0 provides its own include paths via CMake targets
    ${CMAKE_CURRENT_SOURCE_DIR}/src/leveldb/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/leveldb/helpers/memenv
    ${CMAKE_CURRENT_SOURCE_DIR}/src/secp256k1/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/univalue/include
)

# ===== GLOBAL COMPILE DEFINITIONS =====
add_compile_definitions(
    __STDC_FORMAT_MACROS
)

# Windows OpenSSL 3.5.2 DSA compatibility
if(WIN32 OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_compile_definitions(OPENSSL_NO_DSA)
endif()

# All builds should use the generated config header
    add_compile_definitions(HAVE_CONFIG_H)

# Remove NDEBUG even in Release builds (goldcoin requires assertions)
string(REPLACE "-DNDEBUG" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
string(REPLACE "-DNDEBUG" "" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")

# ===== COMPILER FLAGS =====
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wformat
        -Wformat-security
        -Wno-unused-parameter
        -Wno-unused-function
        -Wno-nonnull-compare
    )
	
	# Suppress GCC 13 standard library false positive for MinGW
      if(MINGW OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
          add_compile_options(-Wno-stringop-overread)
      endif()
	
    # Stack protection unavailable in MinGW runtime
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
        add_compile_options(-fstack-protector-all)
    endif()
    add_compile_definitions(_FORTIFY_SOURCE=2)
endif()

# ===== SUBDIRECTORIES FOR EXTERNAL LIBRARIES =====
add_subdirectory(src/leveldb)

# Suppress 3rd party LevelDB array bounds warnings (legacy skiplist negative indexing)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
      target_compile_options(leveldb PRIVATE -Wno-array-bounds)

      # Additional suppressions for LevelDB Windows-specific warnings
      if(MINGW OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
          target_compile_options(leveldb PRIVATE
              -Wno-reorder                      # Member initialization order
              -Wno-missing-field-initializers   # OVERLAPPED struct initialization
              -Wno-unused-variable              # Unused hasWritten variable
          )
      endif()
  endif()

# secp256k1 v0.6.0 with modern CMake integration - static build with recovery module
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Force static libraries for secp256k1" FORCE)
set(SECP256K1_ENABLE_MODULE_RECOVERY ON CACHE BOOL "Enable recovery module for goldcoind" FORCE)

# Disable secp256k1 tests/benchmarks for all builds (enterprise production builds only)
set(SECP256K1_BUILD_TESTS OFF CACHE BOOL "Disable tests for production builds" FORCE)
set(SECP256K1_BUILD_EXHAUSTIVE_TESTS OFF CACHE BOOL "Disable exhaustive tests for production builds" FORCE)
set(SECP256K1_BUILD_BENCHMARK OFF CACHE BOOL "Disable benchmarks for production builds" FORCE)
set(SECP256K1_BUILD_CTIME_TESTS OFF CACHE BOOL "Disable ctime tests for production builds" FORCE)

# Ensure secp256k1 inherits cross-compilation toolchain
if(CMAKE_CROSSCOMPILING)
    # Force secp256k1 to use the correct toolchain
    set(CMAKE_C_COMPILER_FORCED TRUE CACHE BOOL "Force compiler for secp256k1")
    set(CMAKE_CXX_COMPILER_FORCED TRUE CACHE BOOL "Force compiler for secp256k1")
endif()

add_subdirectory(src/secp256k1)

# Windows requires explicit static library declaration
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(secp256k1 PUBLIC SECP256K1_STATIC)
endif()

set(BUILD_SHARED_LIBS ON CACHE BOOL "Reset to default" FORCE)

add_subdirectory(src/univalue)

# ===== CORE LIBRARIES =====

# bitcoin_crypto
file(GLOB CRYPTO_SOURCES src/crypto/*.cpp)
# scrypt-sse2.cpp unconditionally includes the x86-only <emmintrin.h> and is
# dead code on every platform anyway: everything that calls into it is itself
# gated behind #if defined(USE_SSE2), which nothing in this build ever
# defines. Excluding it fixes non-x86 targets (aarch64, arm) with no change
# in behavior on x86_64/i686.
list(FILTER CRYPTO_SOURCES EXCLUDE REGEX "scrypt-sse2\\.cpp$")
list(APPEND CRYPTO_SOURCES src/crypto/ctaes/ctaes.c)
add_library(bitcoin_crypto STATIC ${CRYPTO_SOURCES})
target_include_directories(bitcoin_crypto PUBLIC src/crypto)

# bitcoin_consensus
add_library(bitcoin_consensus STATIC
    src/arith_uint256.cpp
    src/consensus/merkle.cpp
    src/hash.cpp
    src/primitives/block.cpp
    src/primitives/transaction.cpp
    src/pubkey.cpp
    src/script/bitcoinconsensus.cpp
    src/script/interpreter.cpp
    src/script/script.cpp
    src/script/script_error.cpp
    src/uint256.cpp
)

# bitcoin_util
add_library(bitcoin_util STATIC
    src/support/lockedpool.cpp
    src/support/cleanse.cpp
    src/chainparamsbase.cpp
    src/clientversion.cpp
    src/compat/glibc_sanity.cpp
    src/compat/glibcxx_sanity.cpp
    src/compat/strnlen.cpp
    src/random.cpp
    src/rpc/protocol.cpp
    src/sync.cpp
    src/threadinterrupt.cpp
    src/util.cpp
    src/utilmoneystr.cpp
    src/utilstrencodings.cpp
    src/utiltime.cpp
)

# bitcoin_common
add_library(bitcoin_common STATIC
    src/amount.cpp
    src/base58.cpp
    src/chainparams.cpp
    src/coins.cpp
    src/compressor.cpp
    src/core_read.cpp
    src/core_write.cpp
    src/key.cpp
    src/key_io.cpp
    src/keystore.cpp
    src/netaddress.cpp
    src/netbase.cpp
    src/protocol.cpp
    src/scheduler.cpp
    src/script/sign.cpp
    src/script/standard.cpp
    src/warnings.cpp
)

# bitcoin_server
add_library(bitcoin_server STATIC
    src/addrdb.cpp
    src/addrman.cpp
    src/bloom.cpp
    src/blockencodings.cpp
    src/chain.cpp
    src/checkpoints.cpp
    src/checkpointsync.cpp
    src/httprpc.cpp
    src/httpserver.cpp
    src/init.cpp
    src/dbwrapper.cpp
    src/merkleblock.cpp
    src/miner.cpp
    src/net.cpp
    src/net_processing.cpp
    src/noui.cpp
    src/policy/fees.cpp
    src/policy/policy.cpp
    src/pow.cpp
    src/rest.cpp
    src/rpc/blockchain.cpp
    src/rpc/mining.cpp
    src/rpc/misc.cpp
    src/rpc/net.cpp
    src/rpc/rawtransaction.cpp
    src/rpc/server.cpp
    src/script/sigcache.cpp
    src/script/ismine.cpp
    src/timedata.cpp
    src/torcontrol.cpp
    src/txdb.cpp
    src/txmempool.cpp
    src/ui_interface.cpp
    src/validation.cpp
    src/validationinterface.cpp
    src/versionbits.cpp
)

# Add wallet support to bitcoin_server if enabled
if(BUILD_WALLET)
    target_compile_definitions(bitcoin_server PRIVATE ENABLE_WALLET)
endif()

# bitcoin_wallet (conditional)
if(BUILD_WALLET)
    add_library(bitcoin_wallet STATIC
        src/wallet/crypter.cpp
        src/wallet/db.cpp
        src/wallet/rpcdump.cpp
        src/wallet/rpcwallet.cpp
        src/wallet/wallet.cpp
        src/wallet/walletdb.cpp
        src/wallet/migrate.cpp
    )
    target_compile_definitions(bitcoin_wallet PRIVATE ENABLE_WALLET)
endif()

# bitcoin_zmq (conditional)
if(WITH_ZMQ)
    pkg_check_modules(ZMQ REQUIRED libzmq>=4)
    add_library(bitcoin_zmq STATIC
        src/zmq/zmqabstractnotifier.cpp
        src/zmq/zmqnotificationinterface.cpp
        src/zmq/zmqpublishnotifier.cpp
    )
    target_include_directories(bitcoin_zmq PRIVATE ${ZMQ_INCLUDE_DIRS})
    target_compile_definitions(bitcoin_zmq PRIVATE ENABLE_ZMQ)
endif()

# ===== EXECUTABLES =====

# goldcoind
add_executable(goldcoind src/goldcoind.cpp)
target_link_libraries(goldcoind
    bitcoin_server
    bitcoin_common
    bitcoin_util
    bitcoin_consensus
    bitcoin_crypto
    memenv  # memenv depends on leveldb, so it comes first
    leveldb
    secp256k1
    univalue
    # No boost libraries - using C++23 std library
    OpenSSL::SSL
    OpenSSL::Crypto
    ${PLATFORM_LIBS}
    Threads::Threads
)

if(BUILD_WALLET)
    # bitcoin_crypto must come after bitcoin_wallet for AES symbols
    target_link_libraries(goldcoind bitcoin_wallet ${BDB_LIBRARIES} bitcoin_crypto)
    target_compile_definitions(goldcoind PRIVATE ENABLE_WALLET)
endif()

if(WITH_ZMQ)
    target_link_libraries(goldcoind bitcoin_zmq ${ZMQ_LIBRARIES})
    target_compile_definitions(goldcoind PRIVATE ENABLE_ZMQ)
endif()

# Static build for Linux
if(BUILD_STATIC AND NOT WIN32 AND NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set_target_properties(goldcoind PROPERTIES 
        LINK_FLAGS "-static -pthread"
    )
    # Force static linking for all libraries
    target_link_libraries(goldcoind 
        -static
        -pthread
        -ldl
    )
    message(STATUS "Building static goldcoind for Linux")
endif()

# Make goldcoind standalone on Windows - OUTSHINE THE SUN!
if(WIN32 OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set_target_properties(goldcoind PROPERTIES 
        LINK_FLAGS "-static -static-libgcc -static-libstdc++ -Wl,--whole-archive -lwinpthread -Wl,--no-whole-archive"
    )
    # Force static runtime
    target_link_libraries(goldcoind 
        -static
        -static-libgcc
        -static-libstdc++
    )
endif()

# goldcoin-cli
add_executable(goldcoin-cli 
    src/goldcoin-cli.cpp
    src/rpc/client.cpp
)
target_link_libraries(goldcoin-cli
    bitcoin_util
    bitcoin_crypto
    univalue
    # No boost libraries - using C++23 std library
    OpenSSL::SSL
    OpenSSL::Crypto
    ${PLATFORM_LIBS}
)

# Static build for Linux CLI
if(BUILD_STATIC AND NOT WIN32 AND NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set_target_properties(goldcoin-cli PROPERTIES 
        LINK_FLAGS "-static -pthread"
    )
    target_link_libraries(goldcoin-cli 
        -static
        -pthread
        -ldl
    )
endif()

# Make goldcoin-cli standalone on Windows - OUTSHINE THE SUN!
if(WIN32 OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set_target_properties(goldcoin-cli PROPERTIES 
        LINK_FLAGS "-static -static-libgcc -static-libstdc++ -Wl,--whole-archive -lwinpthread -Wl,--no-whole-archive"
    )
    # Force static runtime
    target_link_libraries(goldcoin-cli 
        -static
        -static-libgcc
        -static-libstdc++
    )
endif()

# goldcoin-tx
add_executable(goldcoin-tx src/goldcoin-tx.cpp)
target_link_libraries(goldcoin-tx
    bitcoin_common
    bitcoin_util
    bitcoin_consensus
    bitcoin_crypto
    secp256k1
    univalue
    # No boost libraries - using C++23 std library
    OpenSSL::SSL
    OpenSSL::Crypto
)

# ===== QT GUI (Qt 6.9) =====
if(BUILD_GUI)
    message(STATUS "Building Qt 6.9 GUI")
    
    # Qt autogen features
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/src/qt/forms")
    
    # Find Qt6 - Qt6_DIR is already set if needed
    find_package(Qt6 6.2 REQUIRED COMPONENTS Core Gui Widgets Network)
    message(STATUS "Found Qt version: ${Qt6_VERSION}")
    
    # Collect Qt sources
    file(GLOB QT_SOURCES src/qt/*.cpp)
    file(GLOB QT_HEADERS src/qt/*.h)
    
    # Remove test files
    list(REMOVE_ITEM QT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/test/test_main.cpp)
    list(REMOVE_ITEM QT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/bitcoin-qt.cpp)
    # goldcoin.cpp contains main() and should only be in the executable, not the library
    list(REMOVE_ITEM QT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/goldcoin.cpp)
    
    # Remove Mac-specific files on Linux
    if(NOT APPLE)
        list(REMOVE_ITEM QT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/macdockiconhandler.mm)
        list(REMOVE_ITEM QT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/moc_macdockiconhandler.cpp)
        list(REMOVE_ITEM QT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/macdockiconhandler.h)
    endif()
    
    # Qt resources
    set(QT_RESOURCES src/qt/goldcoin.qrc)
    
    # UI forms - CRITICAL: must be added to target sources
    file(GLOB QT_FORMS src/qt/forms/*.ui)
    
    # BIP70 removed - deprecated since 2018, security risk
    # list(APPEND QT_SOURCES src/qt/paymentrequest.pb.cc)
    
    # Add RPC client for Qt console
    list(APPEND QT_SOURCES src/rpc/client.cpp)
    
    # Create bitcoin_qt library
    add_library(bitcoin_qt STATIC
        ${QT_SOURCES}
        ${QT_HEADERS}
        ${QT_RESOURCES}
        ${QT_FORMS}  # CRITICAL for AUTOUIC
    )
    
    # Files that include src/ui_interface.h (NOT generated ui_*.h)
    set_property(SOURCE
        src/qt/goldcoin.cpp
        src/qt/goldcoingui.cpp
        src/qt/clientmodel.cpp
        src/qt/paymentserver.cpp
        src/qt/splashscreen.cpp
        src/qt/transactionview.cpp
        src/qt/walletmodel.cpp
        src/qt/walletview.cpp
        PROPERTY SKIP_AUTOUIC ON
    )
    
    target_include_directories(bitcoin_qt PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/qt
        ${CMAKE_BINARY_DIR}/bitcoin_qt_autogen/include  # For AUTOUIC output
    )
    
    target_link_libraries(bitcoin_qt
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        bitcoin_server
        bitcoin_common
        bitcoin_util
        bitcoin_consensus
        bitcoin_crypto
        leveldb
        memenv
        secp256k1
        univalue
        # No boost libraries - using C++23 std library
        OpenSSL::SSL
        OpenSSL::Crypto
    )
    
    if(BUILD_WALLET)
        target_link_libraries(bitcoin_qt bitcoin_wallet ${BDB_LIBRARIES})
        target_compile_definitions(bitcoin_qt PRIVATE ENABLE_WALLET)
    endif()
    
    # Main Qt executable
    add_executable(goldcoin-qt src/qt/goldcoin.cpp)
    
    # Build library list with proper order
    set(QT_LINK_LIBS
        bitcoin_qt
        bitcoin_server
    )
    
    if(BUILD_WALLET)
        list(APPEND QT_LINK_LIBS bitcoin_wallet ${BDB_LIBRARIES})
        target_compile_definitions(goldcoin-qt PRIVATE ENABLE_WALLET)
    endif()
    
    # Add remaining libraries in dependency order
    list(APPEND QT_LINK_LIBS
        bitcoin_common
        bitcoin_util
        bitcoin_consensus
        bitcoin_crypto  # Must come after wallet for AES256CBC symbols
        leveldb
        memenv
        secp256k1
        univalue
        # No boost libraries - using C++23 std library
        OpenSSL::SSL
        OpenSSL::Crypto
    )
    
    # Link libraries in correct order for HarfBuzz/FreeType compatibility
    list(APPEND QT_LINK_LIBS
        ${PLATFORM_LIBS}
        Threads::Threads
    )
    
    # CRITICAL: Don't use FreeType from depends - it conflicts with system HarfBuzz
    # System HarfBuzz requires system FreeType with color font support
    # The system versions are automatically linked by Qt6
    
    target_link_libraries(goldcoin-qt ${QT_LINK_LIBS})
    
    # Add missing Windows dependencies for Qt static linking
    if(WIN32)
        # Allow override of Qt bundled lib directory (for enterprise builds)
        if(DEFINED QT_BUNDLED_LIB_DIR)
            # Use provided path (from Makefile or command line)
            message(STATUS "Using provided Qt bundled lib dir: ${QT_BUNDLED_LIB_DIR}")
        elseif(EXISTS "${Qt6_DIR}/../../../lib/libQt6BundledPcre2.a")
            # Auto-detect from Qt6_DIR
            set(QT_BUNDLED_LIB_DIR "${Qt6_DIR}/../../../lib")
            message(STATUS "Auto-detected Qt bundled lib dir: ${QT_BUNDLED_LIB_DIR}")
        else()
            # Fallback (will likely fail, but user will get clear error)
            set(QT_BUNDLED_LIB_DIR "/path/to/qt6-win-build/lib")
            message(WARNING "Qt bundled lib directory not found. Set QT_BUNDLED_LIB_DIR variable.")
        endif()
        
        target_link_libraries(goldcoin-qt 
            # Qt bundled libraries (must come before Windows libraries)
            ${QT_BUNDLED_LIB_DIR}/libQt6BundledPcre2.a
            ${QT_BUNDLED_LIB_DIR}/libQt6BundledLibjpeg.a
            ${QT_BUNDLED_LIB_DIR}/libQt6BundledZLIB.a
            ${QT_BUNDLED_LIB_DIR}/libQt6BundledFreetype.a
            ${QT_BUNDLED_LIB_DIR}/libQt6BundledHarfbuzz.a
            ${QT_BUNDLED_LIB_DIR}/libQt6BundledLibpng.a
            # Essential Windows system libraries
            -lws2_32      # Windows sockets
            -lwsock32     # Windows sockets legacy
            -lwinmm       # Windows multimedia
            -lgdi32       # Windows GDI
            -lcomdlg32    # Common dialogs
            -loleaut32    # OLE automation
            -limm32       # Input method
            -lole32       # OLE
            -luuid        # UUID
            -ladvapi32    # Advanced API
            -lshell32     # Shell API
            -luser32      # User interface
            -lkernel32    # Kernel
            -lmingw32     # MinGW runtime
        )
    endif()
    
    if(WITH_ZMQ)
        target_link_libraries(goldcoin-qt bitcoin_zmq ${ZMQ_LIBRARIES})
        target_compile_definitions(goldcoin-qt PRIVATE ENABLE_ZMQ)
    endif()
    
    
    if(WIN32)
        set_target_properties(goldcoin-qt PROPERTIES 
            WIN32_EXECUTABLE TRUE
            LINK_FLAGS "-static -static-libgcc -static-libstdc++ -Wl,--as-needed"
        )
    endif()
endif()

# ===== TESTS =====
if(BUILD_TESTS)
    enable_testing()
    
    file(GLOB TEST_SOURCES src/test/*.cpp)
    list(REMOVE_ITEM TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/test/test_bitcoin_fuzzy.cpp)
    
    add_executable(test_goldcoin ${TEST_SOURCES})
    target_link_libraries(test_goldcoin
        bitcoin_server
        bitcoin_common
        bitcoin_util
        bitcoin_consensus
        bitcoin_crypto
        leveldb
        memenv
        secp256k1
        univalue
        # No boost libraries - using C++23 std library
        OpenSSL::SSL
        OpenSSL::Crypto
        ${PLATFORM_LIBS}
        Threads::Threads
    )
    
    if(BUILD_WALLET)
        file(GLOB WALLET_TEST_SOURCES src/wallet/test/*.cpp)
        target_sources(test_goldcoin PRIVATE ${WALLET_TEST_SOURCES})
        target_link_libraries(test_goldcoin bitcoin_wallet ${BDB_LIBRARIES})
        target_compile_definitions(test_goldcoin PRIVATE ENABLE_WALLET)
    endif()
    
    add_test(NAME test_goldcoin COMMAND test_goldcoin)
endif()

# ===== INSTALLATION =====
install(TARGETS goldcoind goldcoin-cli goldcoin-tx
    RUNTIME DESTINATION bin
)

if(BUILD_GUI)
    install(TARGETS goldcoin-qt
        RUNTIME DESTINATION bin
    )
endif()


# ===== STATUS MESSAGES =====
message(STATUS "")
message(STATUS "Goldcoin configuration:")
message(STATUS "  Version:        ${PROJECT_VERSION}")
message(STATUS "  Build type:     ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ standard:   ${CMAKE_CXX_STANDARD}")
message(STATUS "  Use depends:    ${USE_DEPENDS}")
message(STATUS "")
message(STATUS "  GUI:            ${BUILD_GUI}")
message(STATUS "  Wallet:         ${BUILD_WALLET}")
message(STATUS "  Tests:          ${BUILD_TESTS}")
message(STATUS "  Benchmarks:     ${BUILD_BENCH}")
message(STATUS "  ZMQ:            ${WITH_ZMQ}")
message(STATUS "")
message(STATUS "  C++23 std lib:  ENABLED (No Boost)")
message(STATUS "  OpenSSL:        ${OPENSSL_VERSION}")
if(BUILD_GUI)
    message(STATUS "  Qt:             ${Qt6_VERSION}")
endif()
message(STATUS "")

# Export compile commands for IDE/analysis
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)