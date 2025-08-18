#!/bin/bash
# OPERATION MODERN PHOENIX - Phase 4: Add [[nodiscard]] attributes
# SCC & LCC Dual-AI Modernization Script

echo "🔥 OPERATION MODERN PHOENIX - PHASE 4 🔥"
echo "Adding [[nodiscard]] attributes to getter functions..."

# Count getter functions
echo "Analyzing getter functions..."

# Phase 4A: Simple getters (GetXxx pattern)
echo "Phase 4A: Marking GetXxx() functions..."
find src -name "*.h" | xargs sed -i 's/^\([[:space:]]*\)\([A-Za-z_][A-Za-z0-9_:<>]*\) Get\([A-Z][A-Za-z0-9_]*\)(/\1[[nodiscard]] \2 Get\3(/g'

# Phase 4B: Boolean checkers (IsXxx, HasXxx pattern)
echo "Phase 4B: Marking IsXxx() and HasXxx() functions..."
find src -name "*.h" | xargs sed -i 's/^\([[:space:]]*\)bool Is\([A-Z][A-Za-z0-9_]*\)(/\1[[nodiscard]] bool Is\2(/g'
find src -name "*.h" | xargs sed -i 's/^\([[:space:]]*\)bool Has\([A-Z][A-Za-z0-9_]*\)(/\1[[nodiscard]] bool Has\2(/g'
find src -name "*.h" | xargs sed -i 's/^\([[:space:]]*\)bool Can\([A-Z][A-Za-z0-9_]*\)(/\1[[nodiscard]] bool Can\2(/g'

# Phase 4C: Size/count functions
echo "Phase 4C: Marking size() and count() functions..."
find src -name "*.h" | xargs sed -i 's/^\([[:space:]]*\)size_t size()/\1[[nodiscard]] size_t size()/g'
find src -name "*.h" | xargs sed -i 's/^\([[:space:]]*\)size_t count()/\1[[nodiscard]] size_t count()/g'
find src -name "*.h" | xargs sed -i 's/^\([[:space:]]*\)int count()/\1[[nodiscard]] int count()/g'

# Phase 4D: Const member functions returning values
echo "Phase 4D: Identifying const member functions..."
grep -r "const;$" src --include="*.h" | grep -E "^\s*[A-Za-z_].*\(" > nodiscard_candidates.txt
echo "Additional candidates saved to nodiscard_candidates.txt"

# Count results
MARKED=$(find src -name "*.h" | xargs grep -c "\[\[nodiscard\]\]" | awk -F: '{sum+=$2} END {print sum}')
echo "✅ Functions marked with [[nodiscard]]: $MARKED"

echo "🔥 PHASE 4 COMPLETE! 🔥"