#!/usr/bin/env python3
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup
import pybind11
import os

# Make paths absolute for robustness
script_dir = os.path.dirname(os.path.abspath(__file__))
src_dir = os.path.abspath(os.path.join(script_dir, '..', 'src'))

# Define the extension module
ext_modules = [
    Pybind11Extension(
        "goldcoin_crypto",
        [
            os.path.join(script_dir, "goldcoin_crypto.cpp"),
            os.path.join(src_dir, "crypto/scrypt.cpp"),
            os.path.join(src_dir, "crypto/hmac_sha256.cpp"),
            os.path.join(src_dir, "crypto/sha256.cpp"),
        ],
        include_dirs=[
            # Path to pybind11 headers
            pybind11.get_cmake_dir() + "/../../../include",
            # Path to goldcoin source headers
            src_dir,
            # System includes for OpenSSL
            "/usr/include/openssl",
        ],
        libraries=["crypto", "ssl"],  # Link against OpenSSL
        cxx_std=11,
        extra_compile_args=['-std=c++11'],  # Explicit C++ standard
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