#!/bin/bash
# Build script for Rust components of Goldcoin

set -e

echo "🦀 Building Goldcoin Rust components..."

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if cargo is installed
if ! command -v cargo &> /dev/null; then
    echo -e "${RED}Error: Cargo is not installed${NC}"
    echo "Please install Rust from https://rustup.rs/"
    exit 1
fi

# Check Rust version (need 1.89+)
RUST_VERSION=$(rustc --version | cut -d' ' -f2)
MIN_VERSION="1.89.0"

if [ "$(printf '%s\n' "$MIN_VERSION" "$RUST_VERSION" | sort -V | head -n1)" != "$MIN_VERSION" ]; then
    echo -e "${RED}Error: Rust $MIN_VERSION or higher is required (found $RUST_VERSION)${NC}"
    echo "Please update Rust with: rustup update"
    exit 1
fi

echo -e "${GREEN}✓ Rust $RUST_VERSION detected${NC}"

# Build mode (debug or release)
BUILD_MODE=${1:-release}

if [ "$BUILD_MODE" = "debug" ]; then
    echo -e "${YELLOW}Building in DEBUG mode${NC}"
    CARGO_FLAGS=""
    TARGET_DIR="target/debug"
else
    echo -e "${GREEN}Building in RELEASE mode${NC}"
    CARGO_FLAGS="--release"
    TARGET_DIR="target/release"
fi

# Build the Rust project
echo "Building goldcoin-core..."
cargo build $CARGO_FLAGS

# Check if build was successful
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Rust build successful${NC}"
else
    echo -e "${RED}✗ Rust build failed${NC}"
    exit 1
fi

# Create symlinks for the libraries in a standard location
mkdir -p ../lib/rust
ln -sf "$(pwd)/$TARGET_DIR/libgoldcoin_ffi.a" ../lib/rust/
ln -sf "$(pwd)/$TARGET_DIR/libgoldcoin_ffi.so" ../lib/rust/ 2>/dev/null || true

echo -e "${GREEN}✓ Libraries linked to lib/rust/${NC}"

# Run tests if requested
if [ "$2" = "test" ]; then
    echo "Running Rust tests..."
    cargo test $CARGO_FLAGS
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ All tests passed${NC}"
    else
        echo -e "${RED}✗ Some tests failed${NC}"
        exit 1
    fi
fi

# Generate documentation if requested
if [ "$2" = "doc" ]; then
    echo "Generating documentation..."
    cargo doc --no-deps --open
fi

echo -e "${GREEN}🎉 Goldcoin Rust components ready!${NC}"
echo ""
echo "Libraries available at:"
echo "  - Static: $TARGET_DIR/libgoldcoin_ffi.a"
echo "  - Dynamic: $TARGET_DIR/libgoldcoin_ffi.so (if available)"
echo ""
echo "To integrate with C++, link against libgoldcoin_ffi and include rust_bridge.h"