#!/bin/bash
# Boost Detection Script - Run before EVERY commit!
# This script prevents boost regressions

echo "================================================"
echo "     BOOST REGRESSION DETECTION SCRIPT"
echo "================================================"
echo ""

# Check for boost in source files
echo "Checking for boost:: usage in source files..."
BOOST_COUNT=$(grep -r "boost::" src/ --include="*.cpp" --include="*.h" 2>/dev/null | grep -v "//" | grep -v "^\s*\*" | wc -l)

if [ $BOOST_COUNT -gt 0 ]; then
    echo "❌ ERROR: Found $BOOST_COUNT boost references!"
    echo ""
    echo "Files containing boost:"
    echo "----------------------"
    grep -r "boost::" src/ --include="*.cpp" --include="*.h" | grep -v "//" | grep -v "^\s*\*" | cut -d: -f1 | sort | uniq
    echo ""
    echo "Detailed occurrences:"
    echo "--------------------"
    grep -r "boost::" src/ --include="*.cpp" --include="*.h" -n | grep -v "//" | grep -v "^\s*\*"
    echo ""
    echo "REPLACEMENT GUIDE:"
    echo "=================="
    echo "boost::filesystem::path     → fs::path"
    echo "boost::filesystem::exists   → fsbridge::Exists"
    echo "boost::filesystem::remove   → fsbridge::Remove"
    echo "boost::variant              → std::variant"
    echo "boost::apply_visitor        → std::visit"
    echo "boost::static_visitor       → (use lambda with std::visit)"
    echo "boost::get                  → std::get_if"
    echo "boost::function             → std::function"
    echo "boost::bind                 → std::bind_front"
    echo "boost::thread               → std::thread"
    echo "boost::thread_group         → std::vector<std::thread>"
    echo "boost::scoped_array         → std::unique_ptr<T[]>"
    echo "boost::optional             → std::optional"
    echo "boost::signals2             → Qt signals or std::function callbacks"
    echo ""
    echo "❌ COMMIT BLOCKED: Remove all boost before committing!"
    exit 1
else
    echo "✅ SUCCESS: No boost dependencies found!"
    echo ""
    
    # Also check for common boost includes
    echo "Checking for boost includes..."
    BOOST_INCLUDES=$(grep -r "#include.*boost" src/ --include="*.cpp" --include="*.h" 2>/dev/null | grep -v "//" | wc -l)
    
    if [ $BOOST_INCLUDES -gt 0 ]; then
        echo "⚠️  WARNING: Found $BOOST_INCLUDES boost #include statements:"
        grep -r "#include.*boost" src/ --include="*.cpp" --include="*.h" | grep -v "//"
        echo ""
        echo "These includes should be removed!"
        exit 1
    else
        echo "✅ No boost includes found!"
    fi
    
    echo ""
    echo "================================================"
    echo "     CODEBASE IS BOOST-FREE! 🎉"
    echo "     Safe to commit!"
    echo "================================================"
fi