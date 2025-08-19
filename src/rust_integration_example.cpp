// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

/**
 * Example integration of Rust networking and consensus with C++23
 * 
 * This demonstrates the hybrid C++23/Rust architecture where:
 * - Networking is handled by Rust with async/await and QUIC
 * - Consensus validation is parallelized in Rust
 * - C++23 provides the main application logic and GUI
 */

#include "rust_bridge.h"
#include <iostream>
#include <format>
#include <print>
#include <thread>
#include <chrono>
#include <expected>

using namespace goldcoin::rust;
using namespace std::chrono_literals;

/// Example: Using Rust network manager from C++23
void networkExample() {
    std::println("🦀 Starting Rust Network Manager Example");
    
    // Create network configuration
    FFINetworkConfig config {
        .listen_port = 51241,
        .max_peers = 125,
        .max_outbound = 8,
        .protocol_version = 70018,
        .user_agent = "/Goldcoin:0.17.0(C++23+Rust)/",
        .relay = true,
        .services = 1  // NODE_NETWORK
    };
    
    try {
        // Create network manager (RAII - auto cleanup)
        RustNetworkManager network(config);
        
        // Start the network
        auto start_result = network.start();
        if (!start_result) {
            std::println(stderr, "❌ Failed to start network: {}", start_result.error());
            return;
        }
        
        std::println("✅ Network started successfully!");
        
        // Monitor network for 10 seconds
        for (int i = 0; i < 10; ++i) {
            auto stats = network.getStats();
            std::println("📊 Network Stats - Peers: {} | Sent: {} bytes | Recv: {} bytes",
                        stats.peer_count, stats.bytes_sent, stats.bytes_recv);
            
            std::this_thread::sleep_for(1s);
        }
        
        // Stop the network (also happens automatically in destructor)
        auto stop_result = network.stop();
        if (!stop_result) {
            std::println(stderr, "⚠️ Warning: {}", stop_result.error());
        }
        
    } catch (const std::exception& e) {
        std::println(stderr, "❌ Exception: {}", e.what());
    }
}

/// Example: Using Rust consensus validation from C++23
void consensusExample() {
    std::println("\n🦀 Starting Rust Consensus Validation Example");
    
    try {
        // Create chain state
        RustChainState chain;
        
        // Create a test block header
        FFIBlockHeader header {
            .version = 1,
            .prev_block = {0}, // Genesis parent
            .merkle_root = {0},
            .timestamp = static_cast<uint32_t>(std::time(nullptr)),
            .bits = 0x207fffff,  // Easy difficulty for testing
            .nonce = 12345
        };
        
        // Create coinbase transaction
        std::vector<uint8_t> coinbase_data = {
            0x01, 0x00, 0x00, 0x00,  // version
            0x01,                     // input count
            // ... transaction data ...
        };
        
        std::vector<FFITransaction> transactions;
        transactions.push_back({
            .version = 1,
            .data = coinbase_data.data(),
            .data_len = coinbase_data.size()
        });
        
        // Validate the block
        std::println("🔍 Validating block...");
        auto validate_result = chain.validateBlock(header, transactions);
        
        if (!validate_result) {
            std::println("⚠️ Block validation failed: {}", validate_result.error());
        } else {
            std::println("✅ Block validation passed!");
        }
        
        // Connect the block to chain
        std::println("🔗 Connecting block to chain...");
        auto connect_result = chain.connectBlock(header, transactions, 1);
        
        if (!connect_result) {
            std::println("❌ Failed to connect block: {}", connect_result.error());
        } else {
            std::println("✅ Block connected successfully!");
            
            // Get chain stats
            auto stats = chain.getStats();
            std::println("📊 Chain Stats:");
            std::println("  Height: {}", stats.best_height);
            std::println("  UTXO Count: {}", stats.utxo_count);
            
            // Get best block hash
            auto hash = chain.getBestHash();
            std::print("  Best Hash: ");
            for (auto byte : hash) {
                std::print("{:02x}", byte);
            }
            std::println("");
        }
        
    } catch (const std::exception& e) {
        std::println(stderr, "❌ Exception: {}", e.what());
    }
}

/// Example: Performance comparison between C++ and Rust validation
void performanceComparison() {
    std::println("\n⚡ Performance Comparison: C++ vs Rust");
    
    // Create test data
    const int num_blocks = 1000;
    std::vector<FFIBlockHeader> headers;
    headers.reserve(num_blocks);
    
    for (int i = 0; i < num_blocks; ++i) {
        headers.push_back({
            .version = 1,
            .prev_block = {0},
            .merkle_root = {0},
            .timestamp = static_cast<uint32_t>(std::time(nullptr) + i),
            .bits = 0x207fffff,
            .nonce = static_cast<uint32_t>(i)
        });
    }
    
    // Benchmark Rust validation
    RustChainState rust_chain;
    auto rust_start = std::chrono::high_resolution_clock::now();
    
    for (const auto& header : headers) {
        std::vector<FFITransaction> empty_txs;
        auto result = rust_chain.validateBlock(header, empty_txs);
        // Ignore result for benchmark
    }
    
    auto rust_end = std::chrono::high_resolution_clock::now();
    auto rust_duration = std::chrono::duration_cast<std::chrono::milliseconds>(rust_end - rust_start);
    
    std::println("🦀 Rust validation time: {}ms for {} blocks", 
                rust_duration.count(), num_blocks);
    std::println("   Average: {:.2f}ms per block", 
                static_cast<double>(rust_duration.count()) / num_blocks);
    
    // Note: C++ validation would go here for comparison
    std::println("📝 Note: C++ validation comparison requires linking with existing validation code");
}

/// Example: Using C++23 features with Rust integration
void modernCppFeatures() {
    std::println("\n✨ C++23 Features with Rust Integration");
    
    // Using std::expected for error handling (matches Rust's Result<T, E>)
    auto createNetwork = []() -> std::expected<RustNetworkManager, std::string> {
        FFINetworkConfig config {
            .listen_port = 51241,
            .max_peers = 125,
            .max_outbound = 8,
            .protocol_version = 70018,
            .user_agent = "/Goldcoin:0.17.0/",
            .relay = true,
            .services = 1
        };
        
        try {
            return RustNetworkManager(config);
        } catch (const std::exception& e) {
            return std::unexpected(std::string(e.what()));
        }
    };
    
    // Chain operations using monadic style
    auto result = createNetwork()
        .and_then([](RustNetworkManager& net) { return net.start(); })
        .transform([]() { 
            std::println("✅ Network started via monadic chain!");
            return true;
        });
    
    if (!result) {
        std::println(stderr, "❌ Monadic chain failed: {}", result.error());
    }
    
    // Using std::flat_map (C++23) to store peer info (mirrors Rust's design)
    std::flat_map<uint64_t, std::string> peer_map;
    peer_map[1] = "127.0.0.1:51241";
    peer_map[2] = "192.168.1.100:51241";
    
    std::println("📍 Peer map (C++23 flat_map):");
    for (const auto& [id, addr] : peer_map) {
        std::println("  Peer {}: {}", id, addr);
    }
    
    // Using std::print with formatting (C++23)
    std::println("🎯 This output uses C++23 std::println!");
    std::println("   Formatting: {:>10} | {:<10} | {:^10}", "Right", "Left", "Center");
}

int main() {
    std::println("🚀 Goldcoin C++23/Rust Hybrid Architecture Demo");
    std::println("================================================");
    std::println("The world's first cryptocurrency with C++23 AND Rust!");
    std::println("");
    
    // Run examples
    networkExample();
    consensusExample();
    performanceComparison();
    modernCppFeatures();
    
    std::println("\n🏆 Demo Complete!");
    std::println("Goldcoin is now powered by:");
    std::println("  • C++23 for application logic and GUI");
    std::println("  • Rust 1.89 for networking and consensus");
    std::println("  • QUIC transport for modern P2P");
    std::println("  • Tokio async runtime for performance");
    std::println("  • Zero-cost FFI bridge between languages");
    
    return 0;
}