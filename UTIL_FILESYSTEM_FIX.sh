#!/bin/bash
# EMERGENCY FIX: Replace all boost::filesystem with std::filesystem in util.cpp

echo "🔥 FIXING util.cpp - THE CANCER CORE!"

# Replace all boost::filesystem:: with std::filesystem::
sed -i 's/boost::filesystem::/std::filesystem::/g' src/util.cpp

# Replace specific filesystem types
sed -i 's/boost::filesystem::path/std::filesystem::path/g' src/util.cpp
sed -i 's/boost::filesystem::ifstream/std::ifstream/g' src/util.cpp
sed -i 's/boost::filesystem::filesystem_error/std::filesystem::filesystem_error/g' src/util.cpp
sed -i 's/boost::filesystem::create_directory/std::filesystem::create_directory/g' src/util.cpp
sed -i 's/boost::filesystem::exists/std::filesystem::exists/g' src/util.cpp
sed -i 's/boost::filesystem::is_directory/std::filesystem::is_directory/g' src/util.cpp
sed -i 's/boost::filesystem::remove/std::filesystem::remove/g' src/util.cpp

echo "✅ util.cpp fixed!"
echo "🔥 Now fixing other affected files..."

# Fix other files that use boost::filesystem
for file in src/dbwrapper.cpp src/validation.cpp src/bitcoind.cpp src/bitcoin-cli.cpp src/addrdb.cpp; do
    if [ -f "$file" ]; then
        echo "Fixing $file..."
        sed -i 's/#include <boost\/filesystem\.hpp>/#include <filesystem>/g' "$file"
        sed -i 's/#include <boost\/filesystem\/fstream\.hpp>/#include <fstream>/g' "$file"
        sed -i 's/#include <boost\/filesystem\/operations\.hpp>/#include <filesystem>/g' "$file"
        sed -i 's/boost::filesystem::/std::filesystem::/g' "$file"
        sed -i 's/boost::filesystem/std::filesystem/g' "$file"
    fi
done

echo "✅ ALL FILES FIXED!"
echo "🚀 Ready to rebuild without boost::filesystem!"