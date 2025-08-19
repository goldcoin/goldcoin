// Test program for C++23/Rust network integration
// Mount Shasta Trinity Test #1: Network Connection

#include <iostream>
#include <chrono>
#include <thread>
#include <cstdio>
#include "src/rust_bridge.h"

using namespace std::chrono_literals;

int main() {
    std::cout << "🚀 GOLDCOIN C++23/RUST NETWORK TEST\n";
    std::cout << "====================================\n\n";
    
    // Test 1: Create network manager
    std::cout << "Test 1: Creating Rust network manager from C++...\n";
    
    FFINetworkConfig config {
        .listen_port = 51241,
        .max_peers = 125,
        .max_outbound = 8,
        .protocol_version = 70018,
        .user_agent = "/Goldcoin:0.17.0(C++23+Rust)/",
        .relay = true,
        .services = 1  // NODE_NETWORK
    };
    
    NetworkHandle* network = goldcoin_network_create(&config);
    if (!network) {
        std::cerr << "❌ Failed to create network manager!\n";
        return 1;
    }
    std::cout << "✅ Network manager created successfully!\n\n";
    
    // Test 2: Start the network
    std::cout << "Test 2: Starting QUIC network listener...\n";
    FFIResult start_result = goldcoin_network_start(network);
    
    if (!start_result.success) {
        std::cerr << "❌ Failed to start network: " << 
                     (start_result.error_msg ? start_result.error_msg : "Unknown error") << "\n";
        goldcoin_result_free_error(start_result);
        goldcoin_network_free(network);
        return 1;
    }
    std::cout << "✅ Network started on port 51241!\n\n";
    
    // Test 3: Check peer count
    std::cout << "Test 3: Monitoring network for 5 seconds...\n";
    for (int i = 0; i < 5; ++i) {
        size_t peer_count = goldcoin_network_peer_count(network);
        NetworkStats stats = goldcoin_network_get_stats(network);
        
        printf("  [%ds] Peers: %zu | Sent: %lu bytes | Recv: %lu bytes\n",
               i, peer_count, stats.bytes_sent, stats.bytes_recv);
        
        std::this_thread::sleep_for(1s);
    }
    
    // Test 4: Stop the network
    std::cout << "\nTest 4: Stopping network...\n";
    FFIResult stop_result = goldcoin_network_stop(network);
    
    if (!stop_result.success) {
        std::cerr << "⚠️  Warning during stop: " << 
                     (stop_result.error_msg ? stop_result.error_msg : "Unknown") << "\n";
        goldcoin_result_free_error(stop_result);
    } else {
        std::cout << "✅ Network stopped cleanly!\n";
    }
    
    // Cleanup
    goldcoin_network_free(network);
    
    std::cout << "\n====================================\n";
    std::cout << "🎉 NETWORK TEST COMPLETE!\n";
    std::cout << "The C++23/Rust bridge is working!\n";
    std::cout << "Mount Shasta Trinity SUCCESS! 🏔️🦀⚡\n";
    
    return 0;
}