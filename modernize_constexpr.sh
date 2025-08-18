#!/bin/bash
# OPERATION MODERN PHOENIX - Phase 3: static const → constexpr
# SCC Territory: wallet/rpc/qt

echo "🔥 OPERATION MODERN PHOENIX - PHASE 3 🔥"
echo "Converting static const to constexpr..."

# Count before
BEFORE=$(find src/wallet src/rpc src/qt -name "*.h" -o -name "*.cpp" | xargs grep "static const " | grep -E "(int|unsigned|size_t|uint32_t|uint64_t|int64_t|quint32)" | wc -l)
echo "Found $BEFORE static const candidates"

# Simple numeric constants
echo "Converting numeric constants..."
find src/wallet src/rpc src/qt -name "*.h" -o -name "*.cpp" | while read file; do
    # Basic integer types
    sed -i 's/static const int /static constexpr int /g' "$file"
    sed -i 's/static const unsigned int /static constexpr unsigned int /g' "$file"
    sed -i 's/static const unsigned /static constexpr unsigned /g' "$file"
    sed -i 's/static const size_t /static constexpr size_t /g' "$file"
    sed -i 's/static const uint32_t /static constexpr uint32_t /g' "$file"
    sed -i 's/static const uint64_t /static constexpr uint64_t /g' "$file"
    sed -i 's/static const int64_t /static constexpr int64_t /g' "$file"
    sed -i 's/static const quint32 /static constexpr quint32 /g' "$file"
done

# Count after
AFTER=$(find src/wallet src/rpc src/qt -name "*.h" -o -name "*.cpp" | xargs grep "static const " | grep -E "(int|unsigned|size_t|uint32_t|uint64_t|int64_t|quint32)" | wc -l)
CONVERTED=$((BEFORE - AFTER))

echo "✅ Phase 3 Complete: $CONVERTED static const → constexpr conversions!"
echo "   Remaining: $AFTER (may be non-numeric or complex types)"