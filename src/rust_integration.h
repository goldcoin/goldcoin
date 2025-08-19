// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_RUST_INTEGRATION_H
#define BITCOIN_RUST_INTEGRATION_H

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

// Forward declarations
struct NetworkHandle;
struct ChainHandle;

// FFI structures matching Rust definitions
extern "C" {
    struct FFIResult {
        bool success;
        const char* error_msg;
    };

    struct FFINetworkConfig {
        uint16_t listen_port;
        size_t max_peers;
        size_t max_outbound;
        uint32_t protocol_version;
        const char* user_agent;
        bool relay;
        uint64_t services;
    };

    struct FFIBlockHeader {
        int32_t version;
        uint8_t prev_block[32];
        uint8_t merkle_root[32];
        uint32_t timestamp;
        uint32_t bits;
        uint32_t nonce;
    };

    struct FFITransaction {
        int32_t version;
        const uint8_t* data;
        size_t data_len;
    };

    struct BlockTime {
        uint32_t height;
        int64_t timestamp;
    };

    // Rust FFI functions (goldcoin_ prefix)
    extern NetworkHandle* goldcoin_network_create(const FFINetworkConfig* config);
    extern void goldcoin_network_free(NetworkHandle* handle);
    extern FFIResult goldcoin_network_start(NetworkHandle* handle);
    extern FFIResult goldcoin_network_stop(NetworkHandle* handle);
    extern FFIResult goldcoin_network_connect(NetworkHandle* handle, const char* addr);
    extern size_t goldcoin_network_peer_count(const NetworkHandle* handle);

    extern ChainHandle* goldcoin_chain_create();
    extern void goldcoin_chain_free(ChainHandle* handle);
    extern FFIResult goldcoin_validate_block(ChainHandle* handle, const FFIBlockHeader* header, 
                                             const FFITransaction* txs, size_t tx_count);
    extern FFIResult goldcoin_validate_transaction(ChainHandle* handle, const FFITransaction* tx);
    extern FFIResult goldcoin_network_connect(NetworkHandle* handle, const char* addr);
    extern uint32_t goldcoin_get_best_height(const ChainHandle* handle);

    // Goldcoin-specific functions
    extern uint32_t rust_golden_river(const BlockTime* last_60_blocks, size_t count,
                                      const BlockTime* last_120_blocks, size_t count_120,
                                      uint32_t last_bits, uint32_t bits_60_ago,
                                      uint32_t bits_240_ago, uint32_t height);
    extern bool rust_check_51_defense(uint32_t block_time, uint32_t prev_5th_block_time, 
                                      uint32_t height);
    extern bool rust_check_timestamp_limits(int64_t block_time, int64_t adjusted_time,
                                            uint32_t height, bool is_local);
    extern void rust_add_auto_checkpoint(uint32_t height, const uint8_t* ancestor_10_hash);

    // Utility functions  
    extern void goldcoin_result_free_error(FFIResult result);
}

// C++ wrapper classes for RAII
namespace rust {

class NetworkManager {
private:
    NetworkHandle* handle;
    
public:
    NetworkManager(const FFINetworkConfig& config) {
        handle = goldcoin_network_create(&config);
    }
    
    ~NetworkManager() {
        if (handle) {
            goldcoin_network_free(handle);
        }
    }
    
    // Disable copy
    NetworkManager(const NetworkManager&) = delete;
    NetworkManager& operator=(const NetworkManager&) = delete;
    
    // Enable move
    NetworkManager(NetworkManager&& other) noexcept : handle(other.handle) {
        other.handle = nullptr;
    }
    
    NetworkManager& operator=(NetworkManager&& other) noexcept {
        if (this != &other) {
            if (handle) {
                goldcoin_network_free(handle);
            }
            handle = other.handle;
            other.handle = nullptr;
        }
        return *this;
    }
    
    bool Start() {
        if (!handle) return false;
        FFIResult result = goldcoin_network_start(handle);
        if (!result.success && result.error_msg) {
            goldcoin_result_free_error(result);
        }
        return result.success;
    }
    
    bool Stop() {
        if (!handle) return false;
        FFIResult result = goldcoin_network_stop(handle);
        if (!result.success && result.error_msg) {
            goldcoin_result_free_error(result);
        }
        return result.success;
    }
    
    bool Connect(const std::string& addr) {
        if (!handle) return false;
        FFIResult result = goldcoin_network_connect(handle, addr.c_str());
        if (!result.success && result.error_msg) {
            goldcoin_result_free_error(result);
        }
        return result.success;
    }
    
    size_t GetPeerCount() const {
        return handle ? goldcoin_network_peer_count(handle) : 0;
    }
};

class ChainValidator {
private:
    ChainHandle* handle;
    
public:
    ChainValidator() {
        handle = goldcoin_chain_create();
    }
    
    ~ChainValidator() {
        if (handle) {
            goldcoin_chain_free(handle);
        }
    }
    
    // Disable copy
    ChainValidator(const ChainValidator&) = delete;
    ChainValidator& operator=(const ChainValidator&) = delete;
    
    // Enable move
    ChainValidator(ChainValidator&& other) noexcept : handle(other.handle) {
        other.handle = nullptr;
    }
    
    ChainValidator& operator=(ChainValidator&& other) noexcept {
        if (this != &other) {
            if (handle) {
                goldcoin_chain_free(handle);
            }
            handle = other.handle;
            other.handle = nullptr;
        }
        return *this;
    }
    
    bool ValidateBlock(const FFIBlockHeader& header, 
                       const std::vector<FFITransaction>& txs) {
        if (!handle) return false;
        FFIResult result = goldcoin_validate_block(handle, &header, 
                                                   txs.data(), txs.size());
        if (!result.success && result.error_msg) {
            goldcoin_result_free_error(result);
        }
        return result.success;
    }
    
    bool ValidateTransaction(const FFITransaction& tx) {
        if (!handle) return false;
        FFIResult result = goldcoin_validate_transaction(handle, &tx);
        if (!result.success && result.error_msg) {
            goldcoin_result_free_error(result);
        }
        return result.success;
    }
    
    uint32_t GetBestHeight() const {
        return handle ? goldcoin_get_best_height(handle) : 0;
    }
};

// Goldcoin-specific helpers
inline uint32_t CalculateGoldenRiverDifficulty(
    const std::vector<BlockTime>& last60,
    const std::vector<BlockTime>& last120,
    uint32_t lastBits, uint32_t bits60Ago, 
    uint32_t bits240Ago, uint32_t height) {
    return rust_golden_river(last60.data(), last60.size(),
                            last120.data(), last120.size(),
                            lastBits, bits60Ago, bits240Ago, height);
}

inline bool Check51Defense(uint32_t blockTime, uint32_t prev5thBlockTime, uint32_t height) {
    return rust_check_51_defense(blockTime, prev5thBlockTime, height);
}

inline bool CheckTimestampLimits(int64_t blockTime, int64_t adjustedTime, 
                                uint32_t height, bool isLocal) {
    return rust_check_timestamp_limits(blockTime, adjustedTime, height, isLocal);
}

inline void AddAutoCheckpoint(uint32_t height, const uint8_t* ancestor10Hash) {
    rust_add_auto_checkpoint(height, ancestor10Hash);
}

} // namespace rust

#endif // BITCOIN_RUST_INTEGRATION_H