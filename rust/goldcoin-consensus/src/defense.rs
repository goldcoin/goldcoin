// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! 51% Defense System for Goldcoin
//! 
//! Protects against 51% attacks by detecting rapid block production
//! and temporarily delaying block acceptance when an attack is suspected.

use std::sync::{Arc, RwLock};
use std::time::{SystemTime, UNIX_EPOCH, Duration};
use std::collections::HashMap;
use std::net::SocketAddr;

/// Defense system configuration
pub struct DefenseConfig {
    /// Number of blocks to look back
    pub lookback_blocks: u32,
    
    /// Time window for rapid block detection (seconds)
    pub rapid_block_window: u64,
    
    /// Defense delay duration (seconds)
    pub defense_delay: u64,
    
    /// Checkpoint offset when defense triggered
    pub checkpoint_offset: u32,
}

impl Default for DefenseConfig {
    fn default() -> Self {
        Self {
            lookback_blocks: 5,           // Check last 5 blocks
            rapid_block_window: 600,       // 10 minutes
            defense_delay: 840,            // 14 minutes
            checkpoint_offset: 12,         // Schedule checkpoint 12 blocks ahead
        }
    }
}

/// 51% Defense System
pub struct DefenseSystem {
    /// Configuration
    config: DefenseConfig,
    
    /// Is defense currently active?
    defense_active: Arc<RwLock<bool>>,
    
    /// When defense was activated
    defense_start_time: Arc<RwLock<Option<SystemTime>>>,
    
    /// Blacklisted blocks (height -> hash)
    bad_blocks: Arc<RwLock<HashMap<u32, [u8; 32]>>>,
    
    /// Scheduled checkpoint height
    scheduled_checkpoint: Arc<RwLock<Option<u32>>>,
}

impl DefenseSystem {
    pub fn new(config: DefenseConfig) -> Self {
        Self {
            config,
            defense_active: Arc::new(RwLock::new(false)),
            defense_start_time: Arc::new(RwLock::new(None)),
            bad_blocks: Arc::new(RwLock::new(HashMap::new())),
            scheduled_checkpoint: Arc::new(RwLock::new(None)),
        }
    }
    
    /// Check if defense should be triggered
    pub fn check_block_timing(
        &self,
        current_block_time: u64,
        previous_blocks: &[(u32, u64)], // (height, timestamp) pairs
        current_height: u32,
        peer_addr: Option<&SocketAddr>,
    ) -> DefenseResult {
        // Need at least lookback_blocks in history
        if previous_blocks.len() < self.config.lookback_blocks as usize {
            return DefenseResult::Pass;
        }
        
        // Get the block at lookback distance
        let lookback_block = &previous_blocks[self.config.lookback_blocks as usize - 1];
        let time_diff = current_block_time.saturating_sub(lookback_block.1);
        
        // Check if blocks are coming too fast
        if time_diff < self.config.rapid_block_window {
            // Defense triggered!
            *self.defense_active.write().unwrap() = true;
            *self.defense_start_time.write().unwrap() = Some(SystemTime::now());
            
            // Check if this is from a non-local peer
            if let Some(addr) = peer_addr {
                if !is_local_address(addr) {
                    // Blacklist this block
                    self.bad_blocks.write().unwrap()
                        .insert(current_height, [0; 32]); // TODO: actual block hash
                    
                    // Schedule checkpoint
                    *self.scheduled_checkpoint.write().unwrap() = 
                        Some(current_height + self.config.checkpoint_offset);
                    
                    return DefenseResult::BanPeer(
                        "51% defense triggered - rapid block production detected".to_string()
                    );
                }
            }
            
            return DefenseResult::RejectBlock(
                "Block timestamp violates defense rules".to_string()
            );
        }
        
        DefenseResult::Pass
    }
    
    /// Check if defense delay has expired
    pub fn is_defense_active(&self) -> bool {
        let active = *self.defense_active.read().unwrap();
        if !active {
            return false;
        }
        
        // Check if delay period has expired
        if let Some(start_time) = *self.defense_start_time.read().unwrap() {
            let elapsed = SystemTime::now()
                .duration_since(start_time)
                .unwrap_or(Duration::ZERO);
            
            if elapsed.as_secs() >= self.config.defense_delay {
                // Defense period expired
                *self.defense_active.write().unwrap() = false;
                *self.defense_start_time.write().unwrap() = None;
                return false;
            }
        }
        
        true
    }
    
    /// Check if a block is blacklisted
    pub fn is_block_blacklisted(&self, height: u32, hash: &[u8; 32]) -> bool {
        self.bad_blocks.read().unwrap()
            .get(&height)
            .map(|h| h == hash)
            .unwrap_or(false)
    }
    
    /// Get scheduled checkpoint height if any
    pub fn get_scheduled_checkpoint(&self) -> Option<u32> {
        *self.scheduled_checkpoint.read().unwrap()
    }
    
    /// Clear scheduled checkpoint
    pub fn clear_scheduled_checkpoint(&self) {
        *self.scheduled_checkpoint.write().unwrap() = None;
    }
}

/// Result of defense check
#[derive(Debug)]
pub enum DefenseResult {
    /// Block passes defense checks
    Pass,
    
    /// Reject block but don't ban peer
    RejectBlock(String),
    
    /// Ban peer and reject block
    BanPeer(String),
}

/// Check if address is local
fn is_local_address(addr: &SocketAddr) -> bool {
    match addr.ip() {
        std::net::IpAddr::V4(ip) => {
            ip.is_loopback() || 
            ip.is_private() ||
            ip.octets()[0] == 127
        }
        std::net::IpAddr::V6(ip) => {
            ip.is_loopback()
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_defense_trigger() {
        let defense = DefenseSystem::new(DefenseConfig::default());
        
        // Create block history - 5 blocks in 8 minutes (too fast!)
        let mut blocks = vec![];
        for i in 0..5 {
            blocks.push((100 + i, 1000000 + (i * 96) as u64)); // 96 seconds apart
        }
        
        // This should trigger defense
        let result = defense.check_block_timing(
            1000480, // 480 seconds after first block
            &blocks,
            105,
            None,
        );
        
        match result {
            DefenseResult::RejectBlock(_) => {
                assert!(defense.is_defense_active());
            }
            _ => panic!("Defense should have triggered"),
        }
    }
    
    #[test]
    fn test_defense_expiry() {
        let mut config = DefenseConfig::default();
        config.defense_delay = 1; // 1 second for testing
        
        let defense = DefenseSystem::new(config);
        
        // Trigger defense
        *defense.defense_active.write().unwrap() = true;
        *defense.defense_start_time.write().unwrap() = 
            Some(SystemTime::now() - Duration::from_secs(2));
        
        // Should be expired
        assert!(!defense.is_defense_active());
    }
}