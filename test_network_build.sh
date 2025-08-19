#!/bin/bash
# Build script for network test - C++23/Rust integration

set -e

echo "🔨 Building C++23/Rust Network Test..."
echo "======================================"

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Check if Rust library exists (from SCC's build)
RUST_LIB="rust/target/release/libgoldcoin_ffi.a"
if [ ! -f "$RUST_LIB" ]; then
    echo "⚠️  Rust library not found locally. Building..."
    cd rust
    cargo build --release
    cd ..
fi

echo -e "${BLUE}📦 Compiling C++23 test program...${NC}"

# Compile with C++23 standard
g++ -std=c++23 \
    -O3 \
    -I./src \
    -I./src/util \
    -o test_network \
    src/test_network.cpp \
    rust/target/release/libgoldcoin_ffi.a \
    -lpthread \
    -ldl \
    -lm

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✅ Build successful!${NC}"
    echo ""
    echo "🚀 Ready to run: ./test_network"
    echo ""
    echo "Mount Shasta Trinity ready for testing!"
else
    echo "❌ Build failed"
    exit 1
fi