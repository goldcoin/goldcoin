// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GOLDCOIN_UTIL_EXPECTED_H
#define GOLDCOIN_UTIL_EXPECTED_H

#include <expected>
#include <string>
#include <format>

namespace goldcoin {

// Error types for Goldcoin operations
enum class ValidationError {
    INVALID_INPUTS,
    INSUFFICIENT_FEE,
    DOUBLE_SPEND,
    SCRIPT_FAILED,
    DUST_OUTPUT,
    OVERSIZED_TX,
    COINBASE_MATURITY,
    MEMPOOL_FULL
};

enum class BlockError {
    INVALID_HEADER,
    INVALID_POW,
    TOO_MANY_SIGOPS,
    BAD_MERKLE_ROOT,
    DUPLICATE_BLOCK,
    ORPHAN_BLOCK,
    TIMESTAMP_TOO_FAR
};

// Convert errors to strings using C++23 std::format
inline std::string ErrorString(ValidationError err) {
    switch(err) {
        case ValidationError::INVALID_INPUTS: 
            return "Invalid transaction inputs";
        case ValidationError::INSUFFICIENT_FEE: 
            return "Insufficient transaction fee";
        case ValidationError::DOUBLE_SPEND: 
            return "Double spend detected";
        case ValidationError::SCRIPT_FAILED: 
            return "Script verification failed";
        case ValidationError::DUST_OUTPUT: 
            return "Transaction output below dust threshold";
        case ValidationError::OVERSIZED_TX: 
            return "Transaction exceeds maximum size";
        case ValidationError::COINBASE_MATURITY: 
            return "Coinbase requires 100 confirmations";
        case ValidationError::MEMPOOL_FULL: 
            return "Memory pool is full";
    }
    return "Unknown validation error";
}

inline std::string ErrorString(BlockError err) {
    switch(err) {
        case BlockError::INVALID_HEADER: 
            return "Invalid block header";
        case BlockError::INVALID_POW: 
            return "Proof of work failed";
        case BlockError::TOO_MANY_SIGOPS: 
            return "Block exceeds signature operation limit";
        case BlockError::BAD_MERKLE_ROOT: 
            return "Merkle root mismatch";
        case BlockError::DUPLICATE_BLOCK: 
            return "Block already exists";
        case BlockError::ORPHAN_BLOCK: 
            return "Block parent not found";
        case BlockError::TIMESTAMP_TOO_FAR: 
            return "Block timestamp too far in future";
    }
    return "Unknown block error";
}

// Type aliases for common expected types
template<typename T>
using TxResult = std::expected<T, ValidationError>;

template<typename T>
using BlockResult = std::expected<T, BlockError>;

template<typename T>
using Result = std::expected<T, std::string>;

// Helper function to chain expected operations (monadic style)
template<typename T, typename E, typename F>
auto AndThen(const std::expected<T, E>& exp, F&& func) {
    if (exp.has_value()) {
        return func(exp.value());
    }
    return std::expected<decltype(func(exp.value()).value()), E>(std::unexpected(exp.error()));
}

// C++23 std::print integration for logging errors
template<typename E>
void LogError(const std::unexpected<E>& err) {
    std::println(stderr, "Error: {}", ErrorString(err.error()));
}

} // namespace goldcoin

#endif // GOLDCOIN_UTIL_EXPECTED_H