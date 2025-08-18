#!/bin/bash
# OPERATION MODERN PHOENIX - Phase 1: typedef → using
# SCC & LCC Dual-AI Modernization Script

echo "🔥 OPERATION MODERN PHOENIX - PHASE 1 🔥"
echo "Converting typedef to using declarations..."

# Simple typedef pattern: typedef type name;
SIMPLE_PATTERN='s/^typedef \(.*\) \([A-Za-z_][A-Za-z0-9_]*\);$/using \2 = \1;/g'

# Template typedef pattern: typedef template<...> name;
TEMPLATE_PATTERN='s/^typedef \(.*<.*>\) \([A-Za-z_][A-Za-z0-9_]*\);$/using \2 = \1;/g'

# Function pointer typedef: typedef return (*name)(params);
FUNCPTR_PATTERN='s/^typedef \(.*\) (\*\([A-Za-z_][A-Za-z0-9_]*\))(\(.*\));$/using \2 = \1 (*)(\3);/g'

# Process files based on territory
if [ "$1" == "scc" ]; then
    echo "Processing SCC territory (wallet/rpc/qt)..."
    FILES=$(find src/wallet src/rpc src/qt -name "*.h" -o -name "*.cpp" 2>/dev/null)
elif [ "$1" == "lcc" ]; then
    echo "Processing LCC territory (core)..."
    FILES=$(find src -name "*.h" -o -name "*.cpp" | grep -v "wallet\|rpc\|qt")
else
    echo "Usage: $0 [scc|lcc]"
    exit 1
fi

# Count before
BEFORE=$(echo "$FILES" | xargs grep "^typedef" | wc -l)
echo "Found $BEFORE typedef declarations to modernize"

# Apply conversions
for file in $FILES; do
    if grep -q "^typedef" "$file" 2>/dev/null; then
        echo "  Modernizing: $file"
        sed -i.bak "$SIMPLE_PATTERN" "$file"
        sed -i "$TEMPLATE_PATTERN" "$file"
        sed -i "$FUNCPTR_PATTERN" "$file"
    fi
done

# Count after
AFTER=$(echo "$FILES" | xargs grep "^typedef" | wc -l)
CONVERTED=$((BEFORE - AFTER))

echo "✅ Phase 1 Complete: $CONVERTED typedef declarations modernized!"
echo "   Remaining: $AFTER (may need manual review)"