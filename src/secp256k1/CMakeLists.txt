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

target_compile_definitions(secp256k1 PRIVATE
    USE_NUM_NONE
    USE_FIELD_INV_BUILTIN
    USE_SCALAR_INV_BUILTIN
    ENABLE_MODULE_RECOVERY
)