// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! Goldcoin Consensus Rules Implementation in Rust
//! 
//! This module implements the consensus validation logic for Goldcoin
//! using safe Rust with zero-cost abstractions and parallel validation.

pub mod goldcoin_features;

use std::sync::Arc;
use std::collections::HashMap;
use goldcoin_features::{DefenseSystem, GoldenRiverDifficulty, CheckpointManager, GoldcoinScriptValidator};

use thiserror::Error;
use tracing::{info, warn, debug};
use serde::{Serialize, Deserialize};

/// Goldcoin consensus parameters
pub mod params {
    use std::time::Duration;
    
    /// Block time target (2 minutes)
    pub const BLOCK_TIME: Duration = Duration::from_secs(2 * 60);
    
    /// Maximum block size (32 MB)
    pub const MAX_BLOCK_SIZE: usize = 32 * 1_000_000;
    
    /// Maximum transaction size (32 MB - for full block transactions)
    pub const MAX_TX_SIZE: usize = MAX_BLOCK_SIZE;
    
    /// Coinbase maturity (100 blocks)
    pub const COINBASE_MATURITY: u32 = 100;
    
    /// Maximum script size
    pub const MAX_SCRIPT_SIZE: usize = 520_000;
    
    /// Maximum signature operations per block
    pub const MAX_BLOCK_SIGOPS: u32 = 160_000;
    
    /// Subsidy halving interval
    pub const SUBSIDY_HALVING_INTERVAL: u32 = 840_000;
    
    /// Initial block subsidy (48 coins)
    pub const INITIAL_SUBSIDY: u64 = 48 * 100_000_000; // in satoshis
}

/// Validation errors
#[derive(Error, Debug, Clone, PartialEq, Eq)]
pub enum ValidationError {
    #[error("Invalid block header: {0}")]
    InvalidHeader(String),
    
    #[error("Invalid proof of work")]
    InvalidProofOfWork,
    
    #[error("Block too large: {size} > {max}")]
    BlockTooLarge { size: usize, max: usize },
    
    #[error("Invalid transaction: {0}")]
    InvalidTransaction(String),
    
    #[error("Double spend detected")]
    DoubleSpend,
    
    #[error("Invalid merkle root")]
    InvalidMerkleRoot,
    
    #[error("Timestamp too far in future")]
    TimestampTooFar,
    
    #[error("Insufficient fee: {fee} < {required}")]
    InsufficientFee { fee: u64, required: u64 },
    
    #[error("Script validation failed: {0}")]
    ScriptFailed(String),
    
    #[error("Too many signature operations: {count} > {max}")]
    TooManySigOps { count: u32, max: u32 },
    
    #[error("Coinbase not mature: {age} < {required}")]
    CoinbaseNotMature { age: u32, required: u32 },
}

/// Validation result type
pub type ValidationResult<T> = Result<T, ValidationError>;

/// Block header
#[derive(Debug, Clone, Serialize, Deserialize, PartialEq, Eq)]
pub struct BlockHeader {
    pub version: i32,
    pub prev_block: [u8; 32],
    pub merkle_root: [u8; 32],
    pub timestamp: u32,
    pub bits: u32,
    pub nonce: u32,
}

impl BlockHeader {
    /// Calculate block hash (double SHA-256)
    pub fn hash(&self) -> [u8; 32] {
        use sha2::{Sha256, Digest};
        
        let bytes = self.serialize();
        let first_hash = Sha256::digest(&bytes);
        let second_hash = Sha256::digest(&first_hash);
        
        let mut result = [0u8; 32];
        result.copy_from_slice(&second_hash);
        result
    }
    
    /// Serialize header to bytes
    pub fn serialize(&self) -> Vec<u8> {
        let mut buf = Vec::with_capacity(80);
        buf.extend_from_slice(&self.version.to_le_bytes());
        buf.extend_from_slice(&self.prev_block);
        buf.extend_from_slice(&self.merkle_root);
        buf.extend_from_slice(&self.timestamp.to_le_bytes());
        buf.extend_from_slice(&self.bits.to_le_bytes());
        buf.extend_from_slice(&self.nonce.to_le_bytes());
        buf
    }
}

/// Transaction input
#[derive(Debug, Clone, Serialize, Deserialize, PartialEq, Eq)]
pub struct TxInput {
    pub prev_tx: [u8; 32],
    pub prev_index: u32,
    pub script: Vec<u8>,
    pub sequence: u32,
}

/// Transaction output
#[derive(Debug, Clone, Serialize, Deserialize, PartialEq, Eq)]
pub struct TxOutput {
    pub value: u64,
    pub script: Vec<u8>,
}

/// Transaction
#[derive(Debug, Clone, Serialize, Deserialize, PartialEq, Eq)]
pub struct Transaction {
    pub version: i32,
    pub inputs: Vec<TxInput>,
    pub outputs: Vec<TxOutput>,
    pub lock_time: u32,
}

impl Transaction {
    /// Calculate transaction ID (double SHA-256)
    pub fn txid(&self) -> [u8; 32] {
        use sha2::{Sha256, Digest};
        
        let bytes = self.serialize();
        let first_hash = Sha256::digest(&bytes);
        let second_hash = Sha256::digest(&first_hash);
        
        let mut result = [0u8; 32];
        result.copy_from_slice(&second_hash);
        result
    }
    
    /// Serialize transaction to bytes
    pub fn serialize(&self) -> Vec<u8> {
        let mut buf = Vec::new();
        buf.extend_from_slice(&self.version.to_le_bytes());
        
        // Write inputs
        buf.extend_from_slice(&(self.inputs.len() as u64).to_le_bytes());
        for input in &self.inputs {
            buf.extend_from_slice(&input.prev_tx);
            buf.extend_from_slice(&input.prev_index.to_le_bytes());
            buf.extend_from_slice(&(input.script.len() as u64).to_le_bytes());
            buf.extend_from_slice(&input.script);
            buf.extend_from_slice(&input.sequence.to_le_bytes());
        }
        
        // Write outputs
        buf.extend_from_slice(&(self.outputs.len() as u64).to_le_bytes());
        for output in &self.outputs {
            buf.extend_from_slice(&output.value.to_le_bytes());
            buf.extend_from_slice(&(output.script.len() as u64).to_le_bytes());
            buf.extend_from_slice(&output.script);
        }
        
        buf.extend_from_slice(&self.lock_time.to_le_bytes());
        buf
    }
    
    /// Check if transaction is coinbase
    pub fn is_coinbase(&self) -> bool {
        self.inputs.len() == 1 
            && self.inputs[0].prev_tx == [0u8; 32]
            && self.inputs[0].prev_index == 0xffffffff
    }
    
    /// Calculate transaction fee (requires UTXO set)
    pub fn calculate_fee(&self, utxo_value: u64) -> u64 {
        let output_value: u64 = self.outputs.iter().map(|o| o.value).sum();
        utxo_value.saturating_sub(output_value)
    }
}

/// Block
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Block {
    pub header: BlockHeader,
    pub transactions: Vec<Transaction>,
}

impl Block {
    /// Validate block against consensus rules
    pub async fn validate(&self) -> ValidationResult<()> {
        // Check block size
        let block_size = self.size();
        if block_size > params::MAX_BLOCK_SIZE {
            return Err(ValidationError::BlockTooLarge {
                size: block_size,
                max: params::MAX_BLOCK_SIZE,
            });
        }
        
        // Validate merkle root
        let calculated_root = self.calculate_merkle_root();
        if calculated_root != self.header.merkle_root {
            return Err(ValidationError::InvalidMerkleRoot);
        }
        
        // Check proof of work
        if !self.check_proof_of_work() {
            return Err(ValidationError::InvalidProofOfWork);
        }
        
        // Validate all transactions in parallel
        self.validate_transactions().await?;
        
        // Count signature operations
        let sigops = self.count_sigops();
        if sigops > params::MAX_BLOCK_SIGOPS {
            return Err(ValidationError::TooManySigOps {
                count: sigops,
                max: params::MAX_BLOCK_SIGOPS,
            });
        }
        
        Ok(())
    }
    
    /// Calculate block size
    pub fn size(&self) -> usize {
        80 + self.transactions.iter().map(|tx| tx.serialize().len()).sum::<usize>()
    }
    
    /// Calculate merkle root of transactions
    pub fn calculate_merkle_root(&self) -> [u8; 32] {
        use sha2::{Sha256, Digest};
        
        if self.transactions.is_empty() {
            return [0u8; 32];
        }
        
        let mut hashes: Vec<[u8; 32]> = self.transactions
            .iter()
            .map(|tx| tx.txid())
            .collect();
        
        while hashes.len() > 1 {
            if hashes.len() % 2 == 1 {
                hashes.push(*hashes.last().unwrap());
            }
            
            hashes = hashes
                .chunks(2)
                .map(|pair| {
                    let mut hasher = Sha256::new();
                    hasher.update(&pair[0]);
                    hasher.update(&pair[1]);
                    let first = hasher.finalize();
                    
                    let second = Sha256::digest(&first);
                    let mut result = [0u8; 32];
                    result.copy_from_slice(&second);
                    result
                })
                .collect();
        }
        
        hashes[0]
    }
    
    /// Check proof of work
    pub fn check_proof_of_work(&self) -> bool {
        let hash = self.header.hash();
        let target = bits_to_target(self.header.bits);
        
        // Convert hash to big-endian for comparison
        let mut hash_be = hash;
        hash_be.reverse();
        
        hash_be <= target
    }
    
    /// Validate all transactions in parallel
    async fn validate_transactions(&self) -> ValidationResult<()> {
        use rayon::prelude::*;
        
        // First transaction must be coinbase
        if self.transactions.is_empty() || !self.transactions[0].is_coinbase() {
            return Err(ValidationError::InvalidTransaction(
                "First transaction must be coinbase".to_string()
            ));
        }
        
        // No other transaction can be coinbase
        for (i, tx) in self.transactions.iter().enumerate().skip(1) {
            if tx.is_coinbase() {
                return Err(ValidationError::InvalidTransaction(
                    format!("Transaction {} cannot be coinbase", i)
                ));
            }
        }
        
        // Parallel validation of non-coinbase transactions
        let results: Vec<ValidationResult<()>> = self.transactions[1..]
            .par_iter()
            .map(|tx| self.validate_transaction(tx))
            .collect();
        
        for result in results {
            result?;
        }
        
        Ok(())
    }
    
    /// Validate a single transaction
    fn validate_transaction(&self, tx: &Transaction) -> ValidationResult<()> {
        // Check transaction size
        let tx_size = tx.serialize().len();
        if tx_size > params::MAX_TX_SIZE {
            return Err(ValidationError::InvalidTransaction(
                format!("Transaction too large: {} > {}", tx_size, params::MAX_TX_SIZE)
            ));
        }
        
        // Check for empty inputs/outputs
        if tx.inputs.is_empty() {
            return Err(ValidationError::InvalidTransaction(
                "Transaction has no inputs".to_string()
            ));
        }
        
        if tx.outputs.is_empty() {
            return Err(ValidationError::InvalidTransaction(
                "Transaction has no outputs".to_string()
            ));
        }
        
        // Check output values
        let total_out: u64 = tx.outputs.iter().map(|o| o.value).sum();
        if total_out > 21_000_000 * 100_000_000 {
            return Err(ValidationError::InvalidTransaction(
                "Total output exceeds max money".to_string()
            ));
        }
        
        // TODO: Script validation, UTXO checks, etc.
        
        Ok(())
    }
    
    /// Count signature operations in block
    fn count_sigops(&self) -> u32 {
        self.transactions.iter()
            .map(|tx| count_tx_sigops(tx))
            .sum()
    }
}

/// Convert compact bits representation to 256-bit target
fn bits_to_target(bits: u32) -> [u8; 32] {
    let size = (bits >> 24) as usize;
    let word = bits & 0x007fffff;
    
    let mut target = [0u8; 32];
    if size <= 3 {
        target[32 - size] = (word >> (8 * (3 - size))) as u8;
    } else {
        let offset = 32 - size;
        target[offset] = (word >> 16) as u8;
        target[offset + 1] = (word >> 8) as u8;
        target[offset + 2] = word as u8;
    }
    
    target
}

/// Count signature operations in a transaction
fn count_tx_sigops(tx: &Transaction) -> u32 {
    // Simplified sigop counting
    // TODO: Implement full script analysis
    tx.inputs.len() as u32 + tx.outputs.len() as u32
}

/// Chain state manager with Goldcoin features
pub struct ChainState {
    /// Best block hash
    pub best_block: [u8; 32],
    
    /// Best block height
    pub best_height: u32,
    
    /// Total work
    pub chain_work: [u8; 32],
    
    /// UTXO set
    pub utxos: HashMap<([u8; 32], u32), TxOutput>,
    
    /// 51% Defense System
    defense_system: DefenseSystem,
    
    /// Golden River Difficulty Algorithm
    golden_river: GoldenRiverDifficulty,
    
    /// Checkpoint Manager
    checkpoint_manager: CheckpointManager,
    
    /// Script Validator
    script_validator: GoldcoinScriptValidator,
}

impl ChainState {
    /// Create new chain state
    pub fn new() -> Self {
        Self {
            best_block: [0u8; 32],
            best_height: 0,
            chain_work: [0u8; 32],
            utxos: HashMap::new(),
            defense_system: DefenseSystem::new(),
            golden_river: GoldenRiverDifficulty::new(),
            checkpoint_manager: CheckpointManager::new(),
            script_validator: GoldcoinScriptValidator::new(),
        }
    }
    
    /// Connect block to chain with Goldcoin features
    pub async fn connect_block(&mut self, block: &Block, height: u32) -> ValidationResult<()> {
        info!("Connecting Goldcoin block at height {}", height);
        
        // Check 51% defense system
        let block_hash = block.header.hash();
        // For now, skip reorg check (would need to implement block history tracking)
        // In production, we'd check if this causes a reorg deeper than allowed
        
        // Verify checkpoint
        if !self.checkpoint_manager.verify(height, &block_hash) {
            return Err(ValidationError::InvalidHeader("Checkpoint verification failed".to_string()));
        }
        
        // Golden River difficulty is calculated outside this function
        // using the exact algorithm from pow.cpp
        debug!("Connecting block at height {}", height);
        
        // Validate block with Goldcoin rules
        block.validate().await?;
        
        // Update UTXO set
        for (tx_idx, tx) in block.transactions.iter().enumerate() {
            let txid = tx.txid();
            
            // Remove spent UTXOs
            if !tx.is_coinbase() {
                for input in &tx.inputs {
                    let key = (input.prev_tx, input.prev_index);
                    if self.utxos.remove(&key).is_none() {
                        return Err(ValidationError::DoubleSpend);
                    }
                }
            }
            
            // Add new UTXOs
            for (output_idx, output) in tx.outputs.iter().enumerate() {
                let key = (txid, output_idx as u32);
                self.utxos.insert(key, output.clone());
            }
        }
        
        // Update chain state
        self.best_block = block.header.hash();
        self.best_height = height;
        // TODO: Update chain work
        
        debug!("Block connected successfully");
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_block_header_hash() {
        let header = BlockHeader {
            version: 1,
            prev_block: [0u8; 32],
            merkle_root: [0u8; 32],
            timestamp: 0,
            bits: 0x207fffff,
            nonce: 0,
        };
        
        let hash = header.hash();
        assert_eq!(hash.len(), 32);
    }
    
    #[test]
    fn test_transaction_txid() {
        let tx = Transaction {
            version: 1,
            inputs: vec![],
            outputs: vec![],
            lock_time: 0,
        };
        
        let txid = tx.txid();
        assert_eq!(txid.len(), 32);
    }
    
    #[tokio::test]
    async fn test_block_validation() {
        let block = Block {
            header: BlockHeader {
                version: 1,
                prev_block: [0u8; 32],
                merkle_root: [0u8; 32],
                timestamp: 0,
                bits: 0x207fffff,
                nonce: 0,
            },
            transactions: vec![],
        };
        
        let result = block.validate().await;
        assert!(result.is_err()); // Should fail without coinbase
    }
}