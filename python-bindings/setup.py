#!/usr/bin/env python3
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup
import pybind11

# Define the extension module
ext_modules = [
    Pybind11Extension(
        "goldcoin_crypto",
        [
            "goldcoin_crypto.cpp",
            "../src/crypto/scrypt.cpp",
            "../src/crypto/hmac_sha256.cpp",
            "../src/crypto/sha256.cpp",
        ],
        include_dirs=[
            # Path to pybind11 headers
            pybind11.get_cmake_dir() + "/../../../include",
            # Path to goldcoin source headers
            "../src",
            # System includes for OpenSSL
            "/usr/include/openssl",
        ],
        libraries=["crypto", "ssl"],  # Link against OpenSSL
        cxx_std=11,
        define_macros=[
            ("PY_SSIZE_T_CLEAN", None),
        ],
    ),
]

setup(
    name="goldcoin_crypto",
    version="0.1.0",
    author="Goldcoin Project",
    description="Native Python bindings for Goldcoin crypto functions",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    python_requires=">=3.8",
    install_requires=["pybind11>=2.6.0"],
)