cmake_minimum_required(VERSION 3.16)
project(leveldb)

# Platform-specific source files
set(LEVELDB_SOURCES
    db/builder.cc
    db/c.cc
    db/dbformat.cc
    db/db_impl.cc
    db/db_iter.cc
    db/dumpfile.cc
    db/filename.cc
    db/log_reader.cc
    db/log_writer.cc
    db/memtable.cc
    db/repair.cc
    db/table_cache.cc
    db/version_edit.cc
    db/version_set.cc
    db/write_batch.cc
    table/block_builder.cc
    table/block.cc
    table/filter_block.cc
    table/format.cc
    table/iterator.cc
    table/merger.cc
    table/table_builder.cc
    table/table.cc
    table/two_level_iterator.cc
    util/arena.cc
    util/bloom.cc
    util/cache.cc
    util/coding.cc
    util/comparator.cc
    util/crc32c.cc
    util/env.cc  # Base class implementations - REQUIRED for all platforms
    util/filter_policy.cc
    util/hash.cc
    util/histogram.cc
    util/logging.cc
    util/options.cc
    util/status.cc
)

# Platform-specific configuration
if(WIN32 OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
    list(APPEND LEVELDB_SOURCES
        util/env_win.cc
        port/port_win.cc
    )
else()
    list(APPEND LEVELDB_SOURCES
        util/env_posix.cc
        port/port_posix.cc
    )
endif()

add_library(leveldb STATIC ${LEVELDB_SOURCES})

target_include_directories(leveldb PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/port
)
target_include_directories(leveldb PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/util
)

# Platform-specific compile definitions
if(WIN32 OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(leveldb PRIVATE
        LEVELDB_PLATFORM_WINDOWS
        OS_WIN
        _CRT_SECURE_NO_WARNINGS
        LEVELDB_ATOMIC_PRESENT
    )
else()
    target_compile_definitions(leveldb PRIVATE
        LEVELDB_PLATFORM_POSIX
        OS_LINUX
        LEVELDB_ATOMIC_PRESENT
    )
endif()

add_library(memenv STATIC helpers/memenv/memenv.cc)
target_include_directories(memenv PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/port
    ${CMAKE_CURRENT_SOURCE_DIR}/util
)
# Platform-specific compile definitions for memenv
if(WIN32 OR CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(memenv PRIVATE
        LEVELDB_PLATFORM_WINDOWS
        OS_WIN
        _CRT_SECURE_NO_WARNINGS
        LEVELDB_ATOMIC_PRESENT
    )
else()
    target_compile_definitions(memenv PRIVATE
        LEVELDB_PLATFORM_POSIX
        OS_LINUX
        LEVELDB_ATOMIC_PRESENT
    )
endif()
target_link_libraries(memenv leveldb)