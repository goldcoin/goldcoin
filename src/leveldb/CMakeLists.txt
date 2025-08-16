cmake_minimum_required(VERSION 3.16)
project(leveldb)

add_library(leveldb STATIC
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
    util/env.cc
    util/env_posix.cc
    util/filter_policy.cc
    util/hash.cc
    util/histogram.cc
    util/logging.cc
    util/options.cc
    util/status.cc
    port/port_posix.cc
)

target_include_directories(leveldb PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/port
)
target_include_directories(leveldb PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/util
)

target_compile_definitions(leveldb PRIVATE
    LEVELDB_PLATFORM_POSIX
    OS_LINUX
    LEVELDB_ATOMIC_PRESENT
)

add_library(memenv STATIC helpers/memenv/memenv.cc)
target_include_directories(memenv PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/port
    ${CMAKE_CURRENT_SOURCE_DIR}/util
)
target_compile_definitions(memenv PRIVATE
    LEVELDB_PLATFORM_POSIX
    OS_LINUX
    LEVELDB_ATOMIC_PRESENT
)
target_link_libraries(memenv leveldb)