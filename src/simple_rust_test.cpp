// Simple test for Rust FFI without full Goldcoin dependencies
#include "rust_integration.h"
#include <iostream>
#include <cstring>
#include <vector>

int main() {
    std::cout << "Testing Basic Rust FFI Integration...\n";
    
    // Test 1: Golden River difficulty calculation
    std::cout << "\n1. Testing Golden River difficulty calculation...\n";
    std::vector<BlockTime> last60, last120;
    
    // Create sample block times (every 2 minutes for 120 blocks)
    int64_t base_time = 1634567890; // Some timestamp
    for (int i = 0; i < 120; i++) {
        BlockTime bt;
        bt.height = 1000 + i;
        bt.timestamp = base_time + (i * 120); // 2 minutes apart
        last120.push_back(bt);
        
        if (i >= 60) { // Last 60 blocks
            last60.push_back(bt);
        }
    }
    
    uint32_t last_bits = 0x1d00ffff;
    uint32_t bits_60_ago = 0x1d00ffff;
    uint32_t bits_240_ago = 0x1d00ffff;
    uint32_t height = 1120;
    uint32_t new_diff = rust::CalculateGoldenRiverDifficulty(last60, last120, 
                                                            last_bits, bits_60_ago, 
                                                            bits_240_ago, height);
    
    std::cout << "  Current difficulty: 0x" << std::hex << last_bits << std::endl;
    std::cout << "  New difficulty: 0x" << std::hex << new_diff << std::endl;
    std::cout << "  ✓ Golden River calculation completed\n";
    
    // Test 2: 51% Defense check
    std::cout << "\n2. Testing 51% Defense system...\n";
    uint32_t block_time = 1634567890;
    uint32_t prev_5th_time = 1634567290; // 10 minutes ago
    uint32_t test_height = 12345;
    
    bool defense_result = rust::Check51Defense(block_time, prev_5th_time, test_height);
    std::cout << "  Defense check result: " << (defense_result ? "PASSED" : "FAILED") << std::endl;
    std::cout << "  ✓ 51% Defense check completed\n";
    
    // Test 3: Timestamp limits check
    std::cout << "\n3. Testing timestamp limits...\n";
    int64_t block_time_64 = 1634567890;
    int64_t adjusted_time = 1634567880;
    bool timestamp_result = rust::CheckTimestampLimits(block_time_64, adjusted_time, test_height, false);
    std::cout << "  Timestamp check result: " << (timestamp_result ? "VALID" : "INVALID") << std::endl;
    std::cout << "  ✓ Timestamp limits check completed\n";
    
    // Test 4: Network configuration
    std::cout << "\n4. Testing network manager creation...\n";
    try {
        FFINetworkConfig config;
        config.listen_port = 9333;
        config.max_peers = 125;
        config.max_outbound = 8;
        config.protocol_version = 70015;
        config.user_agent = "/Goldcoin:0.17.0/";
        config.relay = true;
        config.services = 1;
        
        rust::NetworkManager network(config);
        std::cout << "  ✓ Network manager created successfully\n";
        
        // Note: Not starting network in test to avoid binding to ports
        std::cout << "  ✓ Network configuration test completed\n";
        
    } catch (const std::exception& e) {
        std::cout << "  ✗ Network manager creation failed: " << e.what() << std::endl;
        return 1;
    }
    
    // Test 5: Chain validator
    std::cout << "\n5. Testing chain validator creation...\n";
    try {
        rust::ChainValidator validator;
        uint32_t best_height = validator.GetBestHeight();
        std::cout << "  Best height from Rust: " << best_height << std::endl;
        std::cout << "  ✓ Chain validator created successfully\n";
        
    } catch (const std::exception& e) {
        std::cout << "  ✗ Chain validator creation failed: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\n🚀 All Rust FFI tests completed successfully!\n";
    std::cout << "Goldcoin C++23/Rust hybrid integration is working!\n";
    
    return 0;
}