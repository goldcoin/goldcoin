#!/bin/bash
# OPERATION MODERN PHOENIX - Phase 2: NULL → nullptr
# SCC & LCC Dual-AI Modernization Script

echo "🔥 OPERATION MODERN PHOENIX - PHASE 2 🔥"
echo "Converting NULL to nullptr..."

# Count total NULL usage
TOTAL=$(find src -name "*.cpp" -o -name "*.h" | xargs grep -w "NULL" | wc -l)
echo "Found $TOTAL NULL instances to modernize"

# Simple NULL replacement
# Word boundary ensures we don't replace NULL in NULLIFY or similar
sed -i 's/\bNULL\b/nullptr/g' $(find src -name "*.cpp" -o -name "*.h")

# Count remaining
REMAINING=$(find src -name "*.cpp" -o -name "*.h" | xargs grep -w "NULL" | wc -l)
CONVERTED=$((TOTAL - REMAINING))

echo "✅ Phase 2 Complete: $CONVERTED NULL → nullptr conversions!"
echo "   Remaining: $REMAINING (in comments or special cases)"

# Remove any #define NULL if present
find src -name "*.h" | xargs grep -l "#define NULL" | while read file; do
    echo "  Removing NULL macro from: $file"
    sed -i '/#define NULL/d' "$file"
done