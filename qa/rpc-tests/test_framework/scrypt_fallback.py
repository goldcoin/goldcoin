#!/usr/bin/env python3
"""
Fallback scrypt implementation that tries to use the native goldcoin_crypto extension.
If not available, falls back to SHA256 for testing.
"""

import hashlib
import os
import sys


def getPoWHash(data):
    """
    Calculate scrypt hash for the given data.
    First tries to use the native goldcoin_crypto extension,
    falls back to SHA256 for testing if unavailable.
    """
    try:
        # Try to import our native extension
        sys.path.insert(0, os.path.join(os.path.dirname(__file__), '../../../python-bindings'))
        import goldcoin_crypto
        return goldcoin_crypto.getPoWHash(data)
    except ImportError:
        # Fallback to SHA256 for testing purposes
        # Note: This is NOT cryptographically equivalent to scrypt!
        return hashlib.sha256(data).digest()