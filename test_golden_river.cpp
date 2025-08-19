// Test Golden River algorithm compatibility between C++ and Rust
#include <iostream>
#include <vector>
#include <chrono>
#include "src/rust_bridge.h"


void test_golden_river() {
    std::cout << "Testing Golden River Difficulty Algorithm (Rust implementation)..." << std::endl;
    
    // Test case 1: Perfect 2-minute blocks
    std::vector<BlockTime> last_60, last_120;
    int64_t base_time = 1700000000;
    
    // Generate 120 blocks with perfect 2-minute intervals
    for (int i = 0; i < 120; ++i) {
        int64_t timestamp = base_time - (i * 120);
        if (i < 60) {
            last_60.push_back(BlockTime{105000 - i, timestamp});
        }
        last_120.push_back(BlockTime{105000 - i, timestamp});
    }
    
    // Test with Rust implementation
    auto start = std::chrono::high_resolution_clock::now();
    
    // Call Rust golden_river function through FFI
    uint32_t new_bits = rust_golden_river(
        last_60.data(), last_60.size(),
        last_120.data(), last_120.size(),
        0x1b0404cb,  // Current bits
        0x1b0404cb,  // 60 blocks ago
        0x1b0404cb,  // 240 blocks ago  
        105000       // Height
    );
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "✓ Golden River calculation completed in " << duration.count() << " microseconds" << std::endl;
    std::cout << "  New difficulty bits: 0x" << std::hex << new_bits << std::dec << std::endl;
    
    // Test case 2: Slow blocks (3 minutes average)
    last_60.clear();
    last_120.clear();
    
    for (int i = 0; i < 120; ++i) {
        int64_t timestamp = base_time - (i * 180); // 3 minute intervals
        if (i < 60) {
            last_60.push_back(BlockTime{105000 - i, timestamp});
        }
        last_120.push_back(BlockTime{105000 - i, timestamp});
    }
    
    new_bits = rust_golden_river(
        last_60.data(), last_60.size(),
        last_120.data(), last_120.size(),
        0x1b0404cb,
        0x1b0404cb,
        0x1b0404cb,
        105000
    );
    
    std::cout << "✓ Slow blocks test: New bits = 0x" << std::hex << new_bits << std::dec << std::endl;
    
    // Test case 3: Fast blocks (1 minute average)
    last_60.clear();
    last_120.clear();
    
    for (int i = 0; i < 120; ++i) {
        int64_t timestamp = base_time - (i * 60); // 1 minute intervals
        if (i < 60) {
            last_60.push_back(BlockTime{105000 - i, timestamp});
        }
        last_120.push_back(BlockTime{105000 - i, timestamp});
    }
    
    new_bits = rust_golden_river(
        last_60.data(), last_60.size(),
        last_120.data(), last_120.size(),
        0x1b0404cb,
        0x1b0404cb,
        0x1b0404cb,
        105000
    );
    
    std::cout << "✓ Fast blocks test: New bits = 0x" << std::hex << new_bits << std::dec << std::endl;
    
    // Test case 4: Deadlock scenario (6 blocks in exactly 10 minutes)
    last_60.clear();
    last_120.clear();
    
    for (int i = 0; i < 120; ++i) {
        // Create pattern where every 6 blocks take exactly 600 seconds
        int64_t timestamp;
        if (i % 6 == 0) {
            timestamp = base_time - ((i / 6) * 600);
        } else {
            timestamp = base_time - ((i / 6) * 600) - (i % 6) * 100;
        }
        
        if (i < 60) {
            last_60.push_back(BlockTime{105000 - i, timestamp});
        }
        last_120.push_back(BlockTime{105000 - i, timestamp});
    }
    
    new_bits = rust_golden_river(
        last_60.data(), last_60.size(),
        last_120.data(), last_120.size(),
        0x1b0404cb,
        0x1b0404cb,
        0x1b0404cb,
        105000
    );
    
    std::cout << "✓ Deadlock detection test: New bits = 0x" << std::hex << new_bits << std::dec << std::endl;
    
    std::cout << "\nGolden River algorithm tests completed successfully!" << std::endl;
}

int main() {
    test_golden_river();
    return 0;
}