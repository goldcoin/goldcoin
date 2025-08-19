// Simple C test for Rust network integration
// Mount Shasta Trinity Test #1: Network Connection

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>

// FFI structures (from rust_bridge.h)
typedef struct NetworkHandle NetworkHandle;

typedef struct {
    bool success;
    const char* error_msg;
} FFIResult;

typedef struct {
    uint16_t listen_port;
    size_t max_peers;
    size_t max_outbound;
    uint32_t protocol_version;
    const char* user_agent;
    bool relay;
    uint64_t services;
} FFINetworkConfig;

typedef struct {
    size_t peer_count;
    uint64_t bytes_sent;
    uint64_t bytes_recv;
    uint64_t messages_sent;
    uint64_t messages_recv;
} NetworkStats;

// FFI functions
extern NetworkHandle* goldcoin_network_create(const FFINetworkConfig* config);
extern FFIResult goldcoin_network_start(NetworkHandle* handle);
extern FFIResult goldcoin_network_stop(NetworkHandle* handle);
extern void goldcoin_network_free(NetworkHandle* handle);
extern size_t goldcoin_network_peer_count(const NetworkHandle* handle);
extern NetworkStats goldcoin_network_get_stats(const NetworkHandle* handle);
extern void goldcoin_result_free_error(FFIResult result);

int main() {
    printf("🚀 GOLDCOIN C/RUST NETWORK TEST\n");
    printf("================================\n\n");
    
    // Test 1: Create network manager
    printf("Test 1: Creating Rust network manager from C...\n");
    
    FFINetworkConfig config = {
        .listen_port = 51241,
        .max_peers = 125,
        .max_outbound = 8,
        .protocol_version = 70018,
        .user_agent = "/Goldcoin:0.17.0(C+Rust)/",
        .relay = true,
        .services = 1  // NODE_NETWORK
    };
    
    NetworkHandle* network = goldcoin_network_create(&config);
    if (!network) {
        fprintf(stderr, "❌ Failed to create network manager!\n");
        return 1;
    }
    printf("✅ Network manager created successfully!\n\n");
    
    // Test 2: Start the network
    printf("Test 2: Starting QUIC network listener...\n");
    FFIResult start_result = goldcoin_network_start(network);
    
    if (!start_result.success) {
        fprintf(stderr, "❌ Failed to start network: %s\n",
                start_result.error_msg ? start_result.error_msg : "Unknown error");
        goldcoin_result_free_error(start_result);
        goldcoin_network_free(network);
        return 1;
    }
    printf("✅ Network started on port 51241!\n\n");
    
    // Test 3: Check peer count
    printf("Test 3: Monitoring network for 5 seconds...\n");
    for (int i = 0; i < 5; ++i) {
        size_t peer_count = goldcoin_network_peer_count(network);
        NetworkStats stats = goldcoin_network_get_stats(network);
        
        printf("  [%ds] Peers: %zu | Sent: %lu bytes | Recv: %lu bytes\n",
               i, peer_count, (unsigned long)stats.bytes_sent, 
               (unsigned long)stats.bytes_recv);
        
        sleep(1);
    }
    
    // Test 4: Stop the network
    printf("\nTest 4: Stopping network...\n");
    FFIResult stop_result = goldcoin_network_stop(network);
    
    if (!stop_result.success) {
        fprintf(stderr, "⚠️  Warning during stop: %s\n",
                stop_result.error_msg ? stop_result.error_msg : "Unknown");
        goldcoin_result_free_error(stop_result);
    } else {
        printf("✅ Network stopped cleanly!\n");
    }
    
    // Cleanup
    goldcoin_network_free(network);
    
    printf("\n================================\n");
    printf("🎉 NETWORK TEST COMPLETE!\n");
    printf("The C/Rust bridge is working!\n");
    printf("Mount Shasta Trinity SUCCESS! 🏔️🦀⚡\n");
    
    return 0;
}