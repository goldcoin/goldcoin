#!/bin/bash
# BOOST BAN ENFORCEMENT SCRIPT
# Run this before/after any git operations to ensure boost stays banned

echo "🔍 BOOST BAN ENFORCEMENT - Scanning for violations..."
echo "=================================================="

VIOLATIONS=0

# Check 1: CMakeLists.txt - No uncommented boost find_package
echo "🔍 Checking CMakeLists.txt for boost find_package..."
if grep -E "^[^#]*find_package.*[Bb]oost" CMakeLists.txt; then
    echo "🚨 VIOLATION: Boost find_package detected in CMakeLists.txt!"
    VIOLATIONS=$((VIOLATIONS + 1))
else
    echo "✅ No boost find_package found"
fi

# Check 2: No boost library linking
echo "🔍 Checking for boost library linking..."
if grep -E "^[^#]*Boost::" CMakeLists.txt; then
    echo "🚨 VIOLATION: Boost library linking detected!"
    VIOLATIONS=$((VIOLATIONS + 1))
else
    echo "✅ No boost library linking found"
fi

# Check 3: No boost include directories
echo "🔍 Checking for boost include directories..."
if grep -E "^[^#]*Boost_INCLUDE_DIRS" CMakeLists.txt; then
    echo "🚨 VIOLATION: Boost include directories detected!"
    VIOLATIONS=$((VIOLATIONS + 1))
else
    echo "✅ No boost include directories found"
fi

# Check 4: Verify status message
echo "🔍 Checking status message..."
if grep -q "C++23 std lib:  ENABLED (No Boost)" CMakeLists.txt; then
    echo "✅ Correct status message found"
else
    echo "🚨 VIOLATION: Missing or incorrect boost-free status message!"
    VIOLATIONS=$((VIOLATIONS + 1))
fi

# Check 5: Verify no autotools boost macros reintroduced
echo "🔍 Checking for autotools boost macros..."
if find . -maxdepth 2 -name "ax_boost_*.m4" 2>/dev/null | grep -v depends; then
    echo "🚨 VIOLATION: Autotools boost macros detected!"
    VIOLATIONS=$((VIOLATIONS + 1))
else
    echo "✅ No autotools boost macros found"
fi

echo "=================================================="

if [ $VIOLATIONS -eq 0 ]; then
    echo "🎊 SUCCESS: No boost violations detected!"
    echo "🛡️ Boost ban is properly enforced"
    echo "🚀 C++23 + Rust architecture is protected"
    exit 0
else
    echo "🚨 CRITICAL: $VIOLATIONS boost violations detected!"
    echo "🚫 BOOST IS PERMANENTLY BANNED from this codebase"
    echo "🔄 Please remove boost references and use C++23 std library"
    echo ""
    echo "For help, see:"
    echo "  - BOOST_BAN_ENFORCEMENT.md"
    echo "  - AUTOTOOLS_PURGE_COMPLETE.md"
    exit 1
fi