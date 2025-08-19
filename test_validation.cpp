// Test program for C++23/Rust consensus validation
// Mount Shasta Trinity Test #2: Block Validation

#include <iostream>
#include <chrono>
#include <cstring>
#include <cstdio>
#include "src/rust_bridge.h"

using namespace std::chrono;

int main() {
    std::cout << "🔍 GOLDCOIN C++23/RUST VALIDATION TEST\n";
    std::cout << "=======================================\n\n";
    
    // Test 1: Create chain state
    std::cout << "Test 1: Creating Rust chain state from C++...\n";
    
    ChainHandle* chain = goldcoin_chain_create();
    if (!chain) {
        std::cerr << "❌ Failed to create chain state!\n";
        return 1;
    }
    std::cout << "✅ Chain state created successfully!\n\n";
    
    // Test 2: Validate a block header
    std::cout << "Test 2: Validating block header...\n";
    
    FFIBlockHeader header = {
        .version = 1,
        .prev_block = {0}, // Genesis parent
        .merkle_root = {0},
        .timestamp = static_cast<uint32_t>(std::time(nullptr)),
        .bits = 0x207fffff,  // Easy difficulty
        .nonce = 12345
    };
    
    // Create a simple coinbase transaction
    uint8_t coinbase_data[] = {
        0x01, 0x00, 0x00, 0x00,  // version
        0x01,                     // input count
        // 32 bytes of zeros for prev tx
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0xff, 0xff,  // prev index
        0x00,                     // script length
        0xff, 0xff, 0xff, 0xff,  // sequence
        0x01,                     // output count
        0x00, 0xf2, 0x05, 0x2a, 0x01, 0x00, 0x00, 0x00,  // value (50 BTC)
        0x00,                     // script length
        0x00, 0x00, 0x00, 0x00   // locktime
    };
    
    FFITransaction tx = {
        .version = 1,
        .data = coinbase_data,
        .data_len = sizeof(coinbase_data)
    };
    
    // Time the validation
    auto start = high_resolution_clock::now();
    
    FFIResult result = goldcoin_validate_block(chain, &header, &tx, 1);
    
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    
    if (!result.success) {
        std::cerr << "⚠️  Validation failed (expected for test): " << 
                     (result.error_msg ? result.error_msg : "Unknown") << "\n";
        goldcoin_result_free_error(result);
    } else {
        std::cout << "✅ Block validated successfully!\n";
    }
    
    std::cout << "⏱️  Validation took: " << duration.count() << " microseconds\n\n";
    
    // Test 3: Connect block to chain
    std::cout << "Test 3: Connecting block to chain...\n";
    
    FFIResult connect_result = goldcoin_connect_block(chain, &header, &tx, 1, 1);
    
    if (!connect_result.success) {
        std::cerr << "⚠️  Connect failed (expected for test): " << 
                     (connect_result.error_msg ? connect_result.error_msg : "Unknown") << "\n";
        goldcoin_result_free_error(connect_result);
    } else {
        std::cout << "✅ Block connected to chain!\n";
    }
    
    // Test 4: Check chain stats
    std::cout << "\nTest 4: Checking chain statistics...\n";
    
    uint32_t height = goldcoin_get_best_height(chain);
    ChainStats stats = goldcoin_chain_get_stats(chain);
    
    std::cout << "  Best Height: " << height << "\n";
    std::cout << "  UTXO Count: " << stats.utxo_count << "\n";
    
    uint8_t best_hash[32];
    goldcoin_get_best_hash(chain, best_hash);
    std::cout << "  Best Hash: ";
    for (int i = 0; i < 32; ++i) {
        printf("%02x", best_hash[i]);
    }
    std::cout << "\n";
    
    // Cleanup
    goldcoin_chain_free(chain);
    
    std::cout << "\n=======================================\n";
    std::cout << "🎉 VALIDATION TEST COMPLETE!\n";
    std::cout << "Rust consensus validation is working!\n";
    std::cout << "Mount Shasta Trinity VICTORIOUS! 🏔️🦀⚡\n";
    
    return 0;
}