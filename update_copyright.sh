#!/bin/bash
# Update copyright headers for The Goldcoin Core developers

echo "📝 Updating copyright headers for The Goldcoin Core developers..."

# Get list of modified files from git
MODIFIED_FILES=$(git diff --name-only HEAD~1 | grep -E '\.(cpp|h)$')

COUNT=0
for file in $MODIFIED_FILES; do
    if [ -f "$file" ]; then
        # Check if file already has Goldcoin copyright
        if grep -q "The Goldcoin Core developers" "$file"; then
            # Update existing Goldcoin copyright year to 2025
            sed -i 's/Copyright (c) 20[0-9][0-9]-20[0-9][0-9] The Goldcoin Core developers/Copyright (c) 2013-2025 The Goldcoin Core developers/g' "$file"
            echo "  Updated: $file"
        else
            # Add Goldcoin copyright after Litecoin (if present) or after Bitcoin
            if grep -q "The Litecoin Core developers" "$file"; then
                # Add after Litecoin line
                sed -i '/The Litecoin Core developers/a\// Copyright (c) 2013-2025 The Goldcoin Core developers' "$file"
                echo "  Added after Litecoin: $file"
            elif grep -q "The Bitcoin Core developers" "$file"; then
                # Add after Bitcoin line
                sed -i '/The Bitcoin Core developers/a\// Copyright (c) 2013-2025 The Goldcoin Core developers' "$file"
                echo "  Added after Bitcoin: $file"
            elif grep -q "Satoshi Nakamoto" "$file"; then
                # File has Satoshi but maybe not Bitcoin Core, add after Satoshi
                sed -i '/Satoshi Nakamoto/a\// Copyright (c) 2013-2025 The Goldcoin Core developers' "$file"
                echo "  Added after Satoshi: $file"
            fi
        fi
        ((COUNT++))
    fi
done

echo "✅ Updated copyright in $COUNT files"