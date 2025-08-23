cmake_minimum_required(VERSION 3.16)
project(wallet-migrate VERSION 0.1.0)

# C++23 standard (cutting edge as always!)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(Threads REQUIRED)

# Berkeley DB 18.1 (new)
find_path(BDB18_INCLUDE_DIR db_cxx.h
    PATHS /usr/include/db18 /usr/local/include/db18
    PATH_SUFFIXES db18
)

find_library(BDB18_LIBRARY
    NAMES db_cxx-18.1 db_cxx
    PATHS /usr/lib /usr/local/lib
)

# For reading old format, we need compatibility layer
# This will be implemented in wallet_compat.cpp

# Source files
set(SOURCES
    main.cpp
    wallet_migrate.cpp
    wallet_migrate.h
    wallet_analyzer.cpp
    wallet_analyzer.h
    wallet_backup.cpp
    wallet_backup.h
    wallet_compat.cpp    # BDB 4.8 compatibility layer
    wallet_compat.h
    utils.cpp
    utils.h
)

# Create executable
add_executable(wallet-migrate ${SOURCES})

# Include directories
target_include_directories(wallet-migrate PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BDB18_INCLUDE_DIR}
    ../../../        # For Goldcoin headers
    ../../../crypto  # For crypto functions
)

# Link libraries
target_link_libraries(wallet-migrate
    ${BDB18_LIBRARY}
    Threads::Threads
    stdc++fs  # filesystem
)

# Compiler flags
target_compile_options(wallet-migrate PRIVATE
    -Wall
    -Wextra
    -Wno-unused-parameter
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:Release>:-O3>
)

# Installation
install(TARGETS wallet-migrate
    RUNTIME DESTINATION bin
)

# Status messages
message(STATUS "")
message(STATUS "Wallet Migration Tool Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  BDB 18.1 Include: ${BDB18_INCLUDE_DIR}")
message(STATUS "  BDB 18.1 Library: ${BDB18_LIBRARY}")
message(STATUS "")