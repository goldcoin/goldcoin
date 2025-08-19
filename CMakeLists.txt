cmake_minimum_required(VERSION 3.21)
project(goldcoin VERSION 0.17.0 LANGUAGES CXX C ASM)

# ===== BUILD OPTIONS =====
option(USE_DEPENDS "Use depends/ prefix (for cross-compile only)" OFF)
option(BUILD_GUI "Build goldcoin-qt GUI with Qt 6.9" ON)
option(BUILD_WALLET "Enable wallet functionality" ON)
option(BUILD_TESTS "Build test suite" OFF)
option(BUILD_BENCH "Build benchmarks" OFF)
option(WITH_ZMQ "Enable ZMQ notifications" OFF)
option(WITH_MINIUPNPC "Enable UPnP support" OFF)

# ===== C++ STANDARD (C++23 with GCC 15.2 - No Boost!) =====
# Force GCC 15.2 for full C++23 support
set(CMAKE_C_COMPILER /usr/bin/gcc-15)
set(CMAKE_CXX_COMPILER /usr/bin/g++-15)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# C++23 specific flags
add_compile_options(-std=c++23 -fconcepts -fcoroutines)

# ===== BUILD TYPE =====
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ===== OUTPUT DIRECTORIES =====
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ===== BUILD CONFIGURATION =====
# For native Linux builds: Use Qt6 from depends but SYSTEM OpenSSL/libs
option(USE_DEPENDS_QT "Use Qt6 from depends (native builds)" OFF)

if(USE_DEPENDS)
    # Full depends mode for cross-compilation
    if(WIN32)
        set(DEPENDS_PREFIX "${CMAKE_SOURCE_DIR}/depends/x86_64-w64-mingw32")
    else()
        set(DEPENDS_PREFIX "${CMAKE_SOURCE_DIR}/depends/${CMAKE_SYSTEM_PROCESSOR}-pc-linux-gnu")
    endif()
    list(APPEND CMAKE_PREFIX_PATH "${DEPENDS_PREFIX}")
    message(STATUS "Cross-compile mode - Using full depends: ${DEPENDS_PREFIX}")
elseif(USE_DEPENDS_QT)
    # Native build with Qt6 from depends ONLY
    set(DEPENDS_PREFIX "${CMAKE_SOURCE_DIR}/depends/${CMAKE_SYSTEM_PROCESSOR}-pc-linux-gnu")
    # Set Qt6_DIR and also add to CMAKE_PREFIX_PATH for bundled libraries
    set(Qt6_DIR "${DEPENDS_PREFIX}/lib/cmake/Qt6")
    list(APPEND CMAKE_PREFIX_PATH "${DEPENDS_PREFIX}/lib/cmake")
    message(STATUS "Native build - Using Qt6 from depends ONLY")
    message(STATUS "Qt6_DIR: ${Qt6_DIR}")
    message(STATUS "DEPENDS_PREFIX: ${DEPENDS_PREFIX}")
else()
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
    find_package(OpenSSL 3.0 REQUIRED)
endif()
message(STATUS "OpenSSL version: ${OPENSSL_VERSION}")

# Version check only for native builds
if(NOT WIN32 AND NOT USE_DEPENDS)
    if(OPENSSL_VERSION VERSION_LESS "3.0.0")
        message(FATAL_ERROR "OpenSSL 3.0+ required but found ${OPENSSL_VERSION}. This is the future - we need modern crypto!")
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
    find_package(PkgConfig REQUIRED)
    
    # libevent with pthreads support
    pkg_check_modules(LIBEVENT REQUIRED libevent)
    pkg_check_modules(LIBEVENT_PTHREADS REQUIRED libevent_pthreads)
    
    # Other system libraries
    pkg_check_modules(PROTOBUF REQUIRED protobuf)
    
    # Don't use system FreeType - causes version conflicts with HarfBuzz
    # We'll use the one from depends which is compatible
    
    set(PLATFORM_LIBS 
        ${LIBEVENT_LIBRARIES} 
        ${LIBEVENT_PTHREADS_LIBRARIES}
        ${PROTOBUF_LIBRARIES} 
        dl
    )
else()
    # Windows libraries
    set(PLATFORM_LIBS 
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
    # Use BDB 18.1 from depends directory
    set(BDB_PREFIX "${CMAKE_PREFIX_PATH}")
    if(NOT BDB_PREFIX OR NOT EXISTS "${BDB_PREFIX}/lib/libdb_cxx-18.1.a")
        # Fallback to project depends directory
        set(BDB_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/depends/x86_64-pc-linux-gnu")
    endif()
    
    if(EXISTS "${BDB_PREFIX}/lib/libdb_cxx-18.1.a")
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
    ${CMAKE_CURRENT_SOURCE_DIR}/src/config
    ${CMAKE_CURRENT_SOURCE_DIR}/src/leveldb/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/leveldb/helpers/memenv
    ${CMAKE_CURRENT_SOURCE_DIR}/src/secp256k1/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/univalue/include
    ${Boost_INCLUDE_DIRS}
)

# ===== GLOBAL COMPILE DEFINITIONS =====
add_compile_definitions(
    HAVE_CONFIG_H
    __STDC_FORMAT_MACROS
    BOOST_SP_USE_STD_ATOMIC
    BOOST_AC_USE_STD_ATOMIC
)

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
        -fstack-protector-all
    )
    add_compile_definitions(_FORTIFY_SOURCE=2)
endif()

# ===== SUBDIRECTORIES FOR EXTERNAL LIBRARIES =====
add_subdirectory(src/leveldb)
add_subdirectory(src/secp256k1)
add_subdirectory(src/univalue)

# ===== CORE LIBRARIES =====

# bitcoin_crypto
file(GLOB CRYPTO_SOURCES src/crypto/*.cpp)
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

# bitcoin_wallet (conditional)
if(BUILD_WALLET)
    add_library(bitcoin_wallet STATIC
        src/wallet/crypter.cpp
        src/wallet/db.cpp
        src/wallet/rpcdump.cpp
        src/wallet/rpcwallet.cpp
        src/wallet/wallet.cpp
        src/wallet/walletdb.cpp
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
add_executable(goldcoind src/bitcoind.cpp)
target_link_libraries(goldcoind
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
    # bitcoin_crypto must come after bitcoin_wallet for AES symbols
    target_link_libraries(goldcoind bitcoin_wallet ${BDB_LIBRARIES} bitcoin_crypto)
    target_compile_definitions(goldcoind PRIVATE ENABLE_WALLET)
endif()

if(WITH_ZMQ)
    target_link_libraries(goldcoind bitcoin_zmq ${ZMQ_LIBRARIES})
    target_compile_definitions(goldcoind PRIVATE ENABLE_ZMQ)
endif()

if(BUILD_RUST AND TARGET goldcoin_rust)
    target_link_libraries(goldcoind goldcoin_rust)
    target_compile_definitions(goldcoind PRIVATE ENABLE_RUST)
endif()

# goldcoin-cli
add_executable(goldcoin-cli 
    src/bitcoin-cli.cpp
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

# goldcoin-tx
add_executable(goldcoin-tx src/bitcoin-tx.cpp)
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
    
    # Remove Mac-specific files on Linux
    if(NOT APPLE)
        list(REMOVE_ITEM QT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/macdockiconhandler.mm)
        list(REMOVE_ITEM QT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/moc_macdockiconhandler.cpp)
        list(REMOVE_ITEM QT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/macdockiconhandler.h)
    endif()
    
    # Qt resources
    set(QT_RESOURCES src/qt/bitcoin.qrc)
    
    # UI forms - CRITICAL: must be added to target sources
    file(GLOB QT_FORMS src/qt/forms/*.ui)
    
    # Add protobuf generated files
    list(APPEND QT_SOURCES src/qt/paymentrequest.pb.cc)
    
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
        src/qt/bitcoin.cpp
        src/qt/bitcoingui.cpp
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
    add_executable(goldcoin-qt src/qt/bitcoin.cpp)
    
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
    
    if(WITH_ZMQ)
        target_link_libraries(goldcoin-qt bitcoin_zmq ${ZMQ_LIBRARIES})
        target_compile_definitions(goldcoin-qt PRIVATE ENABLE_ZMQ)
    endif()
    
    if(BUILD_RUST AND TARGET goldcoin_rust)
        target_link_libraries(goldcoin-qt goldcoin_rust)
        target_compile_definitions(goldcoin-qt PRIVATE ENABLE_RUST)
    endif()
    
    if(WIN32)
        set_target_properties(goldcoin-qt PROPERTIES WIN32_EXECUTABLE TRUE)
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

# ===== RUST INTEGRATION (C++23/Rust Hybrid) =====
option(BUILD_RUST "Build Rust components for networking/consensus" ON)

if(BUILD_RUST)
    # Check for Rust compiler
    find_program(CARGO cargo PATHS $ENV{HOME}/.cargo/bin)
    if(NOT CARGO)
        message(WARNING "Cargo not found - Rust components will not be built")
        set(BUILD_RUST OFF)
    else()
        # Get Rust version
        execute_process(
            COMMAND ${CARGO} --version
            OUTPUT_VARIABLE CARGO_VERSION
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        message(STATUS "Found Cargo: ${CARGO_VERSION}")
        
        # Set Rust build type based on CMake build type
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(RUST_BUILD_TYPE "debug")
            set(RUST_TARGET_DIR "debug")
        else()
            set(RUST_BUILD_TYPE "release")
            set(RUST_TARGET_DIR "release")
        endif()
        
        # Custom target to build Rust components
        add_custom_target(rust_build ALL
            COMMAND ${CMAKE_COMMAND} -E echo "Building Rust components in ${RUST_BUILD_TYPE} mode..."
            COMMAND ${CARGO} build $<$<CONFIG:Release>:--release>
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/rust
            COMMENT "Building Goldcoin Rust components (networking/consensus)"
        )
        
        # Import the Rust library
        add_library(goldcoin_rust STATIC IMPORTED GLOBAL)
        set_target_properties(goldcoin_rust PROPERTIES
            IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/rust/target/${RUST_TARGET_DIR}/libgoldcoin_ffi.a"
        )
        add_dependencies(goldcoin_rust rust_build)
        
        # Add Rust include path
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/rust/goldcoin-ffi/include)
    endif()
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
message(STATUS "  Rust:           ${BUILD_RUST}")
message(STATUS "")
message(STATUS "  Boost:          ${Boost_VERSION}")
message(STATUS "  OpenSSL:        ${OPENSSL_VERSION}")
if(BUILD_GUI)
    message(STATUS "  Qt:             ${Qt6_VERSION}")
endif()
if(BUILD_RUST AND CARGO)
    message(STATUS "  Rust:           ${CARGO_VERSION}")
endif()
message(STATUS "")

# Export compile commands for IDE/analysis
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)