cmake_minimum_required(VERSION 3.16)
project(secp256k1 C)

add_library(secp256k1 STATIC
    src/secp256k1.c
)

target_include_directories(secp256k1 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# secp256k1 configuration now handled by generated libsecp256k1-config.h
# No manual compile definitions needed - config header provides everything