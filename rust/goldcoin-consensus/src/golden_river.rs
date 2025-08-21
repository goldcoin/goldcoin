// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! Golden River Difficulty Adjustment Algorithm
//! 
//! Advanced difficulty adjustment that considers both median and average
//! block times to maintain stable 2-minute block times.

use std::collections::VecDeque;

/// Golden River configuration
pub struct GoldenRiverConfig {
    /// Target block time (seconds)
    pub target_spacing: u64,
    
    /// Number of blocks for median calculation
    pub median_blocks: usize,
    
    /// Number of blocks for average calculation  
    pub average_blocks: usize,
    
    /// Maximum difficulty adjustment per period
    pub max_adjustment: f64,
}

impl Default for GoldenRiverConfig {
    fn default() -> Self {
        Self {
            target_spacing: 120,      // 2 minutes
            median_blocks: 60,         // Last 60 blocks for median
            average_blocks: 120,       // Last 120 blocks for average
            max_adjustment: 2.0,       // Max 2x adjustment
        }
    }
}

/// Golden River difficulty calculator
pub struct GoldenRiver {
    config: GoldenRiverConfig,
}

impl GoldenRiver {
    pub fn new(config: GoldenRiverConfig) -> Self {
        Self { config }
    }
    
    /// Calculate next difficulty using Golden River algorithm
    pub fn calculate_next_difficulty(
        &self,
        block_times: &[u64],      // Block timestamps (newest first)
        current_bits: u32,         // Current difficulty bits
        height: u32,               // Current height
    ) -> u32 {
        // Need enough blocks for calculation
        if block_times.len() < self.config.average_blocks {
            return current_bits;
        }
        
        // Calculate time differences between consecutive blocks
        let mut time_diffs_60: Vec<i64> = Vec::new();
        let mut time_diffs_120: Vec<i64> = Vec::new();
        
        for i in 0..self.config.average_blocks - 1 {
            let diff = (block_times[i] as i64 - block_times[i + 1] as i64).abs();
            
            if i < self.config.median_blocks - 1 {
                time_diffs_60.push(diff);
            }
            time_diffs_120.push(diff);
        }
        
        // Calculate median time (using middle value of sorted list)
        time_diffs_60.sort_unstable();
        let median_time = time_diffs_60[time_diffs_60.len() / 2];
        
        // Calculate average time
        let total: i64 = time_diffs_120.iter().sum();
        let average_time = total / time_diffs_120.len() as i64;
        
        // Use the smaller of median and average (prevents gaming)
        let mut actual_time = median_time.min(average_time);
        
        // Special case: network stall detection
        // If average >= 3 minutes and last 2 blocks took > 20 minutes each
        if average_time >= 180 && 
           time_diffs_120[0] >= 1200 && 
           time_diffs_120[1] >= 1200 {
            // Emergency adjustment - target 4 minutes
            actual_time = 240;
        }
        
        // Deadlock prevention with 51% defense
        if median_time >= 120 {
            actual_time = check_defense_deadlock(block_times, actual_time);
        }
        
        // Calculate adjustment ratio
        let adjustment = self.config.target_spacing as f64 / actual_time as f64;
        
        // Apply limits to prevent extreme adjustments
        let clamped_adjustment = adjustment.max(0.5).min(2.0);
        
        // Calculate new difficulty
        let current_target = bits_to_target(current_bits);
        let new_target = multiply_target(current_target, clamped_adjustment);
        
        target_to_bits(new_target)
    }
}

/// Check for 51% defense deadlock situation
fn check_defense_deadlock(block_times: &[u64], current_time: i64) -> i64 {
    let mut num_defense_triggers = 0;
    
    // Check last 55 sets of 5 blocks
    for i in 1..55 {
        if i + 5 < block_times.len() {
            let time_span = block_times[i] - block_times[i + 5];
            
            // Exactly 600 seconds = defense was triggered
            if time_span == 600 {
                num_defense_triggers += 1;
            }
        }
    }
    
    // If many defense triggers, adjust to prevent deadlock
    if num_defense_triggers >= 5 {
        // Force easier difficulty to break deadlock
        return 180; // 3 minutes
    }
    
    current_time
}

/// Convert difficulty bits to target
fn bits_to_target(bits: u32) -> [u8; 32] {
    let size = (bits >> 24) as usize;
    let word = bits & 0x007fffff;
    
    let mut target = [0u8; 32];
    
    if size <= 3 {
        target[29] = (word >> 16) as u8;
        target[30] = (word >> 8) as u8;
        target[31] = word as u8;
    } else {
        let offset = 32 - size;
        target[offset] = (word >> 16) as u8;
        target[offset + 1] = (word >> 8) as u8;
        target[offset + 2] = word as u8;
    }
    
    target
}

/// Convert target to difficulty bits
fn target_to_bits(target: [u8; 32]) -> u32 {
    let mut size = 32;
    let mut compact = 0u32;
    
    // Find first non-zero byte
    for i in 0..32 {
        if target[i] != 0 {
            size = 32 - i;
            
            if target[i] > 0x7f {
                // Would be negative in sign-magnitude representation
                if size < 32 && target[i + 1] == 0 {
                    // Can shift to make positive
                    compact = (target[i] as u32) << 16;
                    if i + 1 < 31 {
                        compact |= (target[i + 2] as u32) << 8;
                    }
                    if i + 2 < 31 {
                        compact |= target[i + 3] as u32;
                    }
                } else {
                    // Add padding byte
                    size += 1;
                    if i + 1 < 32 {
                        compact = (target[i + 1] as u32) << 8;
                    }
                    if i + 2 < 32 {
                        compact |= target[i + 2] as u32;
                    }
                }
            } else {
                compact = (target[i] as u32) << 16;
                if i + 1 < 32 {
                    compact |= (target[i + 1] as u32) << 8;
                }
                if i + 2 < 32 {
                    compact |= target[i + 2] as u32;
                }
            }
            break;
        }
    }
    
    compact | ((size as u32) << 24)
}

/// Multiply a target by an adjustment factor
fn multiply_target(target: [u8; 32], factor: f64) -> [u8; 32] {
    // Convert to big integer, multiply, convert back
    // Simplified version - in production use proper big int library
    
    // For now, just return a slightly adjusted target
    let mut new_target = target;
    
    if factor > 1.0 {
        // Make easier (increase target)
        for i in (0..31).rev() {
            if new_target[i] < 255 {
                new_target[i] = ((new_target[i] as f64 * factor).min(255.0)) as u8;
                break;
            }
        }
    } else if factor < 1.0 {
        // Make harder (decrease target)  
        for i in (0..32).rev() {
            if new_target[i] > 0 {
                new_target[i] = ((new_target[i] as f64 * factor).max(1.0)) as u8;
                break;
            }
        }
    }
    
    new_target
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_golden_river_stable_blocks() {
        let gr = GoldenRiver::new(GoldenRiverConfig::default());
        
        // Create perfect 2-minute blocks
        let mut block_times = vec![];
        for i in 0..120 {
            block_times.push(1000000 + (120 * (119 - i)) as u64);
        }
        
        // Should maintain current difficulty
        let new_bits = gr.calculate_next_difficulty(
            &block_times,
            0x1d00ffff, // Starting difficulty
            100000,
        );
        
        // Difficulty should stay roughly the same
        assert!((new_bits as i64 - 0x1d00ffff).abs() < 0x10000);
    }
    
    #[test]
    fn test_bits_conversion() {
        let bits = 0x1d00ffff;
        let target = bits_to_target(bits);
        let bits_back = target_to_bits(target);
        
        // Should round-trip (approximately)
        assert!((bits as i64 - bits_back as i64).abs() < 0x100);
    }
}