#!/bin/bash
# Qt6 Modernization Performance Benchmark
# Measures build time and binary size improvements

echo "==================================="
echo "Qt6 Modernization Benchmark Report"
echo "==================================="
echo "Date: $(date)"
echo ""

# Check binary sizes
echo "Binary Size Analysis:"
echo "--------------------"
for exe in goldcoin-qt.exe goldcoind.exe goldcoin-cli.exe goldcoin-tx.exe; do
    if [ -f "build-win64/bin/$exe" ]; then
        size=$(du -h "build-win64/bin/$exe" | cut -f1)
        echo "$exe: $size"
    fi
done
echo ""

# Count modernization metrics
echo "Code Modernization Metrics:"
echo "---------------------------"
echo "Qt version checks removed: $(grep -r "QT_VERSION" src/qt/*.cpp 2>/dev/null | wc -l) remaining"
echo "BOOST_FOREACH instances: $(grep -r "BOOST_FOREACH" src/qt/*.cpp 2>/dev/null | wc -l) remaining"
echo "Range-based for loops: $(grep -r "for.*:.*)" src/qt/*.cpp 2>/dev/null | wc -l) found"
echo "Lambda expressions: $(grep -r "\[.*\].*{" src/qt/*.cpp 2>/dev/null | wc -l) found"
echo "[[maybe_unused]]: $(grep -r "\[\[maybe_unused\]\]" src/qt/*.cpp 2>/dev/null | wc -l) instances"
echo "std::optional usage: $(grep -r "std::optional" src/qt/*.cpp 2>/dev/null | wc -l) instances"
echo ""

# Build time test (incremental)
echo "Incremental Build Performance:"
echo "------------------------------"
touch src/qt/bitcoin.cpp
start_time=$(date +%s)
make -C build-win64 -j16 goldcoin-qt 2>&1 | tail -1
end_time=$(date +%s)
build_time=$((end_time - start_time))
echo "Incremental build time: ${build_time} seconds"
echo ""

# File count analysis
echo "File Statistics:"
echo "----------------"
echo "Total Qt .cpp files: $(find src/qt -name "*.cpp" -type f | wc -l)"
echo "Total Qt .h files: $(find src/qt -name "*.h" -type f | wc -l)"
echo "Modernized files (git): $(git log --oneline --grep="moderniz\|Qt6\|C++20" --since="1 day ago" | wc -l) commits"
echo ""

echo "==================================="
echo "Benchmark Complete!"
echo "==================================="