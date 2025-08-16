cmake_minimum_required(VERSION 3.16)
project(univalue)

add_library(univalue STATIC
    lib/univalue.cpp
    lib/univalue_read.cpp
    lib/univalue_write.cpp
)

target_include_directories(univalue PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)