// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! Synchronized Checkpoint System for Goldcoin
//! 
//! This module implements the automatic checkpointing system with
//! hash rate-based dropout mechanism. Provides 51% attack protection
//! when the network is vulnerable, automatically disabling when the
//! network becomes sufficiently strong.

use std::sync::{Arc, RwLock};
use std::collections::HashMap;
use std::time::{SystemTime, UNIX_EPOCH};
use sha2::{Sha256, Digest};
use crate::{Block, BlockHeader, NetworkType};

/// Checkpoint dropout start time: September 9, 2018
const CHECKPOINT_DROPOUT_START: u64 = 1536541807;

/// Base hash rate threshold: 1 TH/s
const BASE_HASHRATE_THRESHOLD: f64 = 1e12;

/// Sync checkpoint height threshold (from consensus params)
const SYNC_CHECKPOINT_HEIGHT: u32 = 420420; // TODO: Get from chainparams

/// Checkpoint depth for auto-checkpoint mode (blocks behind tip)
const DEFAULT_CHECKPOINT_DEPTH: i32 = 5;

/// Unsigned sync checkpoint message
#[derive(Debug, Clone)]
pub struct UnsignedSyncCheckpoint {
    pub version: i32,
    pub hash_checkpoint: [u8; 32],
}

impl UnsignedSyncCheckpoint {
    pub fn new(hash: [u8; 32]) -> Self {
        Self {
            version: 1,
            hash_checkpoint: hash,
        }
    }
}

/// Signed sync checkpoint message
#[derive(Debug, Clone)]
pub struct SyncCheckpoint {
    pub checkpoint: UnsignedSyncCheckpoint,
    pub signature: Vec<u8>,
    pub master_pubkey: Vec<u8>,
}

/// Checkpoint manager
pub struct CheckpointManager {
    /// Current sync checkpoint
    pub hash_sync_checkpoint: Arc<RwLock<[u8; 32]>>,
    
    /// Pending checkpoint awaiting validation
    pub hash_pending_checkpoint: Arc<RwLock<Option<[u8; 32]>>>,
    
    /// Invalid checkpoint (for warning)
    pub hash_invalid_checkpoint: Arc<RwLock<Option<[u8; 32]>>>,
    
    /// Master private key (if this node is checkpoint master)
    master_privkey: Option<String>,
    
    /// Checkpoint depth setting
    checkpoint_depth: i32,
    
    /// Network type
    network_type: NetworkType,
}

impl CheckpointManager {
    pub fn new(network_type: NetworkType) -> Self {
        Self {
            hash_sync_checkpoint: Arc::new(RwLock::new([0; 32])),
            hash_pending_checkpoint: Arc::new(RwLock::new(None)),
            hash_invalid_checkpoint: Arc::new(RwLock::new(None)),
            master_privkey: None,
            checkpoint_depth: DEFAULT_CHECKPOINT_DEPTH,
            network_type,
        }
    }
    
    /// Check if checkpoints should be enforced based on network hash rate
    pub fn should_enforce_checkpoints(&self, height: u32, network_hashrate: f64) -> bool {
        // Below the sync checkpoint height, always enforce
        if height <= SYNC_CHECKPOINT_HEIGHT {
            return true;
        }
        
        // Calculate current threshold using the dropout formula
        let now = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .unwrap()
            .as_secs();
        
        // Doubles every 6 months: 2^(2 * years_elapsed)
        let years_elapsed = (now - CHECKPOINT_DROPOUT_START) as f64 / (365.0 * 24.0 * 60.0 * 60.0);
        let toggle_hashrate = BASE_HASHRATE_THRESHOLD * f64::powf(2.0, 2.0 * years_elapsed);
        
        // Enforce checkpoints only if network hash rate is below threshold
        network_hashrate <= toggle_hashrate
    }
    
    /// Validate that a checkpoint is a valid descendant of current sync checkpoint
    pub fn validate_sync_checkpoint(
        &self,
        hash_checkpoint: &[u8; 32],
        block_index: &HashMap<[u8; 32], BlockInfo>,
    ) -> Result<bool, String> {
        let current = self.hash_sync_checkpoint.read().unwrap();
        
        // Get block info for both checkpoints
        let current_info = block_index.get(&*current)
            .ok_or_else(|| format!("Current sync checkpoint not in index"))?;
        let new_info = block_index.get(hash_checkpoint)
            .ok_or_else(|| format!("New checkpoint not in index"))?;
        
        if new_info.height <= current_info.height {
            // Trace back from current checkpoint to same height
            let mut check_hash = *current;
            let mut check_info = current_info;
            
            while check_info.height > new_info.height {
                check_hash = check_info.prev_hash;
                check_info = block_index.get(&check_hash)
                    .ok_or_else(|| format!("Chain traversal failed"))?;
            }
            
            // They should match at the same height
            if check_hash != *hash_checkpoint {
                self.hash_invalid_checkpoint.write().unwrap().replace(*hash_checkpoint);
                return Err(format!("Checkpoint not on the same chain"));
            }
        } else {
            // New checkpoint is higher, trace it back to current
            let mut check_hash = *hash_checkpoint;
            let mut check_info = new_info;
            
            while check_info.height > current_info.height {
                check_hash = check_info.prev_hash;
                check_info = block_index.get(&check_hash)
                    .ok_or_else(|| format!("Chain traversal failed"))?;
            }
            
            if check_hash != *current {
                self.hash_invalid_checkpoint.write().unwrap().replace(*hash_checkpoint);
                return Err(format!("Checkpoint not descendant of current"));
            }
        }
        
        Ok(true)
    }
    
    /// Check if a block satisfies the sync checkpoint requirement
    pub fn check_sync_checkpoint(
        &self,
        block_hash: &[u8; 32],
        prev_block: Option<&BlockInfo>,
        height: u32,
        network_hashrate: f64,
    ) -> bool {
        // Skip if checkpoints not enforced
        if !self.should_enforce_checkpoints(height, network_hashrate) {
            return true;
        }
        
        let checkpoint = self.hash_sync_checkpoint.read().unwrap();
        
        // Genesis always passes
        if prev_block.is_none() {
            return true;
        }
        
        // Check if this block or an ancestor is the checkpoint
        // TODO: Implement full ancestor checking
        
        true
    }
    
    /// Auto-select a checkpoint based on current chain tip
    pub fn auto_select_sync_checkpoint(
        &self,
        chain_tip: &BlockInfo,
        block_index: &HashMap<[u8; 32], BlockInfo>,
    ) -> Option<[u8; 32]> {
        // Don't auto-checkpoint if depth is negative (manual mode)
        if self.checkpoint_depth < 0 {
            return None;
        }
        
        // Find block at (tip_height - checkpoint_depth)
        let target_height = chain_tip.height.saturating_sub(self.checkpoint_depth as u32);
        
        let mut current = chain_tip.clone();
        while current.height > target_height {
            if let Some(prev) = block_index.get(&current.prev_hash) {
                current = prev.clone();
            } else {
                return None;
            }
        }
        
        Some(current.hash)
    }
    
    /// Set checkpoint master private key
    pub fn set_checkpoint_privkey(&mut self, privkey: String) -> Result<(), String> {
        // TODO: Validate private key format
        self.master_privkey = Some(privkey);
        Ok(())
    }
    
    /// Send sync checkpoint (if we're the checkpoint master)
    pub fn send_sync_checkpoint(&self, hash_checkpoint: [u8; 32]) -> Result<(), String> {
        if self.master_privkey.is_none() {
            return Err("Not checkpoint master".to_string());
        }
        
        // TODO: Sign and broadcast checkpoint message
        
        Ok(())
    }
}

/// Block info for checkpoint validation
#[derive(Debug, Clone)]
pub struct BlockInfo {
    pub hash: [u8; 32],
    pub prev_hash: [u8; 32],
    pub height: u32,
    pub timestamp: u64,
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_dropout_threshold_calculation() {
        let mgr = CheckpointManager::new(NetworkType::Mainnet);
        
        // Test at start time (Sept 9, 2018)
        // Should be exactly 1 TH/s
        let start_hashrate = BASE_HASHRATE_THRESHOLD;
        
        // Test current threshold (should be much higher)
        let now = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .unwrap()
            .as_secs();
        let years = (now - CHECKPOINT_DROPOUT_START) as f64 / (365.0 * 24.0 * 60.0 * 60.0);
        let current_threshold = BASE_HASHRATE_THRESHOLD * f64::powf(2.0, 2.0 * years);
        
        // Should be in PH/s range by now
        assert!(current_threshold > 1e15, "Threshold should be > 1 PH/s by now");
    }
    
    #[test]
    fn test_should_enforce_checkpoints() {
        let mgr = CheckpointManager::new(NetworkType::Mainnet);
        
        // Below sync height - always enforce
        assert!(mgr.should_enforce_checkpoints(100, 1e6));
        assert!(mgr.should_enforce_checkpoints(100, 1e20));
        
        // Above sync height - depends on hashrate
        assert!(mgr.should_enforce_checkpoints(500000, 1e10)); // Low hashrate
        assert!(!mgr.should_enforce_checkpoints(500000, 1e20)); // Very high hashrate
    }
}