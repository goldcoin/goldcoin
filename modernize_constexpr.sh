#!/bin/bash
# OPERATION MODERN PHOENIX - Phase 3: static const → constexpr
# SCC & LCC Dual-AI Modernization Script

echo "🔥 OPERATION MODERN PHOENIX - PHASE 3 🔥"
echo "Converting static const to constexpr..."

# Count candidates
TOTAL=$(find src -name "*.cpp" -o -name "*.h" | xargs grep -c "static const " | awk -F: '{sum+=$2} END {print sum}')
echo "Found $TOTAL static const candidates"

# Phase 3A: Simple numeric constants
echo "Phase 3A: Converting simple numeric constants..."
find src -name "*.h" -o -name "*.cpp" | xargs sed -i 's/static const int /static constexpr int /g'
find src -name "*.h" -o -name "*.cpp" | xargs sed -i 's/static const unsigned int /static constexpr unsigned int /g'
find src -name "*.h" -o -name "*.cpp" | xargs sed -i 's/static const size_t /static constexpr size_t /g'
find src -name "*.h" -o -name "*.cpp" | xargs sed -i 's/static const uint32_t /static constexpr uint32_t /g'
find src -name "*.h" -o -name "*.cpp" | xargs sed -i 's/static const uint64_t /static constexpr uint64_t /g'
find src -name "*.h" -o -name "*.cpp" | xargs sed -i 's/static const int64_t /static constexpr int64_t /g'

# Phase 3B: Member functions that can be constexpr
echo "Phase 3B: Converting inline const member functions..."
# This needs manual review but we can flag candidates
grep -r "inline.*const.*{.*return.*}" src --include="*.h" > constexpr_function_candidates.txt
echo "Function candidates saved to constexpr_function_candidates.txt"

# Count results
CONVERTED=$(find src -name "*.cpp" -o -name "*.h" | xargs grep -c "static constexpr " | awk -F: '{sum+=$2} END {print sum}')
echo "✅ Converted to constexpr: $CONVERTED"

echo "🔥 PHASE 3 COMPLETE! 🔥"