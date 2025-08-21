#!/bin/bash

echo "=== REMOVING ALL BOOST INCLUDES ==="

# Remove all boost includes and add C++23 replacements
find ../src -type f \( -name "*.cpp" -o -name "*.h" \) -exec sed -i \
    -e '/#include.*<boost\/filesystem/d' \
    -e '/#include.*<boost\/thread/d' \
    -e '/#include.*<boost\/chrono/d' \
    -e '/#include.*<boost\/signals2/d' \
    -e '/#include.*<boost\/algorithm/d' \
    -e '/#include.*<boost\/foreach/d' \
    -e '/#include.*<boost\/variant/d' \
    -e '/#include.*<boost\/optional/d' \
    -e '/#include.*<boost\/multi_index/d' \
    -e '/#include.*<boost\/program_options/d' \
    -e '/#include.*<boost\/tuple/d' \
    -e '/#include.*<boost\/exception/d' \
    -e '/#include.*<boost\/preprocessor/d' \
    -e '/#include.*<boost\/assign/d' \
    -e '/#include.*<boost\/range/d' \
    -e '/#include.*<boost\/bind/d' \
    -e '/#include.*<boost\/function/d' \
    -e '/#include.*<boost\/test/d' \
    -e '/#include.*<boost\/interprocess/d' \
    {} \;

# Add C++23 headers where needed
echo "Adding C++23 replacements..."

# Files that need filesystem
for file in ../src/util.cpp ../src/init.cpp ../src/bitcoind.cpp ../src/qt/bitcoin.cpp ../src/dbwrapper.cpp; do
    if [ -f "$file" ]; then
        if ! grep -q "#include <filesystem>" "$file"; then
            sed -i '1a#include <filesystem>' "$file"
        fi
    fi
done

# Files that need thread
for file in ../src/miner.cpp ../src/net.cpp ../src/net_processing.cpp ../src/bitcoind.cpp; do
    if [ -f "$file" ]; then
        if ! grep -q "#include <thread>" "$file"; then
            sed -i '1a#include <thread>' "$file"
        fi
    fi
done

# Files that need variant
for file in ../src/rpc/protocol.cpp ../src/rpc/protocol.h; do
    if [ -f "$file" ]; then
        if ! grep -q "#include <variant>" "$file"; then
            sed -i '1a#include <variant>' "$file"
        fi
    fi
done

# Files that need optional
for file in ../src/script/standard.cpp ../src/script/standard.h; do
    if [ -f "$file" ]; then
        if ! grep -q "#include <optional>" "$file"; then
            sed -i '1a#include <optional>' "$file"
        fi
    fi
done

echo "=== BOOST REMOVAL COMPLETE ==="