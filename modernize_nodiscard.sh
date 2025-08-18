#!/bin/bash
# OPERATION MODERN PHOENIX - Phase 4: Add [[nodiscard]] attributes
# Focus on wallet/rpc/qt territory

echo "🔥 OPERATION MODERN PHOENIX - PHASE 4 🔥"
echo "Adding [[nodiscard]] attributes to critical functions..."

# Create a list of functions that should have [[nodiscard]]
# These are functions where ignoring the return value is likely a bug

# First, let's find bool-returning validation/check functions
echo "Finding validation and check functions..."
VALIDATION_FUNCS=$(find src/wallet src/rpc src/qt -name "*.h" | xargs grep -E "^[[:space:]]*bool (Is|Has|Can|Should|Check|Verify|Validate)" | grep -v "override" | grep -v "nodiscard" | wc -l)

echo "Found $VALIDATION_FUNCS validation functions to mark"

# Pattern 1: Functions starting with Is/Has/Can/Should that return bool
find src/wallet src/rpc src/qt -name "*.h" | while read file; do
    # Is* functions
    sed -i 's/^\([[:space:]]*\)bool Is/\1[[nodiscard]] bool Is/g' "$file"
    # Has* functions  
    sed -i 's/^\([[:space:]]*\)bool Has/\1[[nodiscard]] bool Has/g' "$file"
    # Can* functions
    sed -i 's/^\([[:space:]]*\)bool Can/\1[[nodiscard]] bool Can/g' "$file"
    # Should* functions
    sed -i 's/^\([[:space:]]*\)bool Should/\1[[nodiscard]] bool Should/g' "$file"
    # Check* functions
    sed -i 's/^\([[:space:]]*\)bool Check/\1[[nodiscard]] bool Check/g' "$file"
    # Verify* functions
    sed -i 's/^\([[:space:]]*\)bool Verify/\1[[nodiscard]] bool Verify/g' "$file"
    # Validate* functions
    sed -i 's/^\([[:space:]]*\)bool Validate/\1[[nodiscard]] bool Validate/g' "$file"
done

# Pattern 2: Get* functions that return values
echo "Finding getter functions..."
find src/wallet src/rpc src/qt -name "*.h" | while read file; do
    # Get* functions returning various types
    sed -i 's/^\([[:space:]]*\)\(int\|unsigned\|size_t\|CAmount\|int64_t\|uint64_t\) Get/\1[[nodiscard]] \2 Get/g' "$file"
done

# Pattern 3: Functions that calculate/compute values
echo "Finding calculation functions..."
find src/wallet src/rpc src/qt -name "*.h" | while read file; do
    # Calculate/Compute functions
    sed -i 's/^\([[:space:]]*\)\(CAmount\|int64_t\|uint64_t\|double\) Calculate/\1[[nodiscard]] \2 Calculate/g' "$file"
    sed -i 's/^\([[:space:]]*\)\(CAmount\|int64_t\|uint64_t\|double\) Compute/\1[[nodiscard]] \2 Compute/g' "$file"
done

# Clean up any double [[nodiscard]] that might have been created
find src/wallet src/rpc src/qt -name "*.h" | xargs sed -i 's/\[\[nodiscard\]\] \[\[nodiscard\]\]/[[nodiscard]]/g'

# Count results
AFTER=$(find src/wallet src/rpc src/qt -name "*.h" | xargs grep -c "\[\[nodiscard\]\]" | awk -F: '{sum+=$2} END {print sum}')

echo "✅ Phase 4 Complete: Added [[nodiscard]] to $AFTER functions!"
echo "   Functions marked: validation, getters, calculations"