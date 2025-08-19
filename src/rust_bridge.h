// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GOLDCOIN_RUST_BRIDGE_H
#define GOLDCOIN_RUST_BRIDGE_H

#include <cstdint>
#include <cstddef>
#include <memory>
#include <string>
#include <array>
#include <expected>
#include <utility>
#include <vector>

extern "C" {

// ========== FORWARD DECLARATIONS ==========

struct NetworkHandle;
struct ChainHandle;

// ========== DATA STRUCTURES ==========

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

struct NetworkStats {
    size_t peer_count;
    uint64_t bytes_sent;
    uint64_t bytes_recv;
    uint64_t messages_sent;
    uint64_t messages_recv;
};

struct ChainStats {
    uint32_t best_height;
    size_t utxo_count;
    uint8_t total_work[32];
};

// ========== NETWORK FUNCTIONS ==========

NetworkHandle* goldcoin_network_create(const FFINetworkConfig* config);
FFIResult goldcoin_network_start(NetworkHandle* handle);
FFIResult goldcoin_network_stop(NetworkHandle* handle);
void goldcoin_network_free(NetworkHandle* handle);
size_t goldcoin_network_peer_count(const NetworkHandle* handle);
NetworkStats goldcoin_network_get_stats(const NetworkHandle* handle);

// ========== CONSENSUS FUNCTIONS ==========

ChainHandle* goldcoin_chain_create();
FFIResult goldcoin_validate_block(
    ChainHandle* handle,
    const FFIBlockHeader* header,
    const FFITransaction* txs,
    size_t tx_count
);
FFIResult goldcoin_connect_block(
    ChainHandle* handle,
    const FFIBlockHeader* header,
    const FFITransaction* txs,
    size_t tx_count,
    uint32_t height
);
uint32_t goldcoin_get_best_height(const ChainHandle* handle);
void goldcoin_get_best_hash(const ChainHandle* handle, uint8_t* out_hash);
void goldcoin_chain_free(ChainHandle* handle);
ChainStats goldcoin_chain_get_stats(const ChainHandle* handle);

// ========== GOLDCOIN-SPECIFIC FUNCTIONS ==========

uint32_t rust_golden_river(
    const BlockTime* last_60_blocks,
    size_t last_60_count,
    const BlockTime* last_120_blocks,
    size_t last_120_count,
    uint32_t last_bits,
    uint32_t bits_60_ago,
    uint32_t bits_240_ago,
    uint32_t height
);

bool rust_check_51_defense(
    uint32_t block_time,
    uint32_t prev_5th_block_time,
    uint32_t height
);

bool rust_check_timestamp_limits(
    int64_t block_time,
    int64_t adjusted_time,
    uint32_t height,
    bool is_local
);

void rust_add_auto_checkpoint(
    uint32_t height,
    const uint8_t* ancestor_10_hash
);

// ========== UTILITY FUNCTIONS ==========

void goldcoin_result_free_error(FFIResult result);

} // extern "C"

// ========== C++23 WRAPPER CLASSES ==========

namespace goldcoin::rust {

using namespace std::string_literals;

/// RAII wrapper for Rust network manager
class RustNetworkManager {
private:
    NetworkHandle* handle = nullptr;
    
public:
    explicit RustNetworkManager(const FFINetworkConfig& config) {
        handle = goldcoin_network_create(&config);
        if (!handle) {
            throw std::runtime_error("Failed to create Rust network manager");
        }
    }
    
    ~RustNetworkManager() {
        if (handle) {
            goldcoin_network_stop(handle);
            goldcoin_network_free(handle);
        }
    }
    
    // Delete copy constructor/assignment
    RustNetworkManager(const RustNetworkManager&) = delete;
    RustNetworkManager& operator=(const RustNetworkManager&) = delete;
    
    // Allow move semantics
    RustNetworkManager(RustNetworkManager&& other) noexcept 
        : handle(std::exchange(other.handle, nullptr)) {}
    
    RustNetworkManager& operator=(RustNetworkManager&& other) noexcept {
        if (this != &other) {
            if (handle) {
                goldcoin_network_stop(handle);
                goldcoin_network_free(handle);
            }
            handle = std::exchange(other.handle, nullptr);
        }
        return *this;
    }
    
    [[nodiscard]] std::expected<void, std::string> start() {
        auto result = goldcoin_network_start(handle);
        if (!result.success) {
            std::string error = result.error_msg ? result.error_msg : "Unknown error";
            goldcoin_result_free_error(result);
            return std::unexpected(error);
        }
        return {};
    }
    
    [[nodiscard]] std::expected<void, std::string> stop() {
        auto result = goldcoin_network_stop(handle);
        if (!result.success) {
            std::string error = result.error_msg ? result.error_msg : "Unknown error";
            goldcoin_result_free_error(result);
            return std::unexpected(error);
        }
        return {};
    }
    
    [[nodiscard]] size_t getPeerCount() const {
        return goldcoin_network_peer_count(handle);
    }
    
    [[nodiscard]] NetworkStats getStats() const {
        return goldcoin_network_get_stats(handle);
    }
};

/// RAII wrapper for Rust chain state
class RustChainState {
private:
    ChainHandle* handle = nullptr;
    
public:
    RustChainState() {
        handle = goldcoin_chain_create();
        if (!handle) {
            throw std::runtime_error("Failed to create Rust chain state");
        }
    }
    
    ~RustChainState() {
        if (handle) {
            goldcoin_chain_free(handle);
        }
    }
    
    // Delete copy constructor/assignment
    RustChainState(const RustChainState&) = delete;
    RustChainState& operator=(const RustChainState&) = delete;
    
    // Allow move semantics
    RustChainState(RustChainState&& other) noexcept 
        : handle(std::exchange(other.handle, nullptr)) {}
    
    RustChainState& operator=(RustChainState&& other) noexcept {
        if (this != &other) {
            if (handle) {
                goldcoin_chain_free(handle);
            }
            handle = std::exchange(other.handle, nullptr);
        }
        return *this;
    }
    
    [[nodiscard]] std::expected<void, std::string> validateBlock(
        const FFIBlockHeader& header,
        const std::vector<FFITransaction>& txs
    ) {
        auto result = goldcoin_validate_block(
            handle, 
            &header, 
            txs.data(), 
            txs.size()
        );
        
        if (!result.success) {
            std::string error = result.error_msg ? result.error_msg : "Unknown error";
            goldcoin_result_free_error(result);
            return std::unexpected(error);
        }
        return {};
    }
    
    [[nodiscard]] std::expected<void, std::string> connectBlock(
        const FFIBlockHeader& header,
        const std::vector<FFITransaction>& txs,
        uint32_t height
    ) {
        auto result = goldcoin_connect_block(
            handle, 
            &header, 
            txs.data(), 
            txs.size(),
            height
        );
        
        if (!result.success) {
            std::string error = result.error_msg ? result.error_msg : "Unknown error";
            goldcoin_result_free_error(result);
            return std::unexpected(error);
        }
        return {};
    }
    
    [[nodiscard]] uint32_t getBestHeight() const {
        return goldcoin_get_best_height(handle);
    }
    
    [[nodiscard]] std::array<uint8_t, 32> getBestHash() const {
        std::array<uint8_t, 32> hash;
        goldcoin_get_best_hash(handle, hash.data());
        return hash;
    }
    
    [[nodiscard]] ChainStats getStats() const {
        return goldcoin_chain_get_stats(handle);
    }
};

/// Factory function to create network manager with default config
[[nodiscard]] inline std::unique_ptr<RustNetworkManager> createDefaultNetworkManager() {
    FFINetworkConfig config {
        .listen_port = 51241,
        .max_peers = 125,
        .max_outbound = 8,
        .protocol_version = 70018,
        .user_agent = "/Goldcoin:0.17.0(C++23+Rust)/",
        .relay = true,
        .services = 1  // NODE_NETWORK
    };
    
    return std::make_unique<RustNetworkManager>(config);
}

} // namespace goldcoin::rust

#endif // GOLDCOIN_RUST_BRIDGE_H