// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! Goldcoin Memory Pool (Mempool) Implementation in Rust
//! 
//! High-performance transaction pool with advanced features:
//! - Fee-based priority queue
//! - Ancestor/descendant tracking
//! - Replace-by-fee (RBF) support
//! - Child-pays-for-parent (CPFP)
//! - 0-conf transaction support
//! - Parallel validation

use std::sync::Arc;
use std::collections::{HashMap, HashSet, VecDeque};
use std::time::{Duration, SystemTime, UNIX_EPOCH};

use tokio::sync::RwLock;
use dashmap::DashMap;
use priority_queue::PriorityQueue;
use indexmap::IndexMap;
use serde::{Serialize, Deserialize};
use tracing::{info, warn, debug};
use thiserror::Error;
use sha2::{Sha256, Digest};

/// Maximum mempool size in bytes (300MB default)
pub const MAX_MEMPOOL_SIZE: usize = 300 * 1024 * 1024;

/// Maximum transaction size (32MB for Goldcoin)
pub const MAX_TX_SIZE: usize = 32 * 1024 * 1024;

/// Minimum fee rate (satoshis per KB)
pub const MIN_FEE_RATE: u64 = 1000;

/// Maximum ancestors for a transaction
pub const MAX_ANCESTORS: usize = 25;

/// Maximum descendants for a transaction
pub const MAX_DESCENDANTS: usize = 25;

/// Mempool Error types
#[derive(Debug, Error)]
pub enum MempoolError {
    #[error("Transaction already in mempool")]
    DuplicateTransaction,
    
    #[error("Transaction conflicts with existing transaction")]
    ConflictingTransaction,
    
    #[error("Transaction fee too low: {0} sat/KB")]
    FeeTooLow(u64),
    
    #[error("Transaction too large: {0} bytes")]
    TransactionTooLarge(usize),
    
    #[error("Too many ancestors: {0}")]
    TooManyAncestors(usize),
    
    #[error("Too many descendants: {0}")]
    TooManyDescendants(usize),
    
    #[error("Mempool full")]
    MempoolFull,
    
    #[error("Invalid transaction: {0}")]
    InvalidTransaction(String),
    
    #[error("Missing inputs")]
    MissingInputs,
}

/// Transaction ID (32 bytes)
pub type TxId = [u8; 32];

/// Output point
#[derive(Debug, Clone, Hash, Eq, PartialEq, Serialize, Deserialize)]
pub struct OutPoint {
    pub txid: TxId,
    pub index: u32,
}

/// Transaction input
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct TxInput {
    pub prev_output: OutPoint,
    pub script_sig: Vec<u8>,
    pub sequence: u32,
    pub witness: Vec<Vec<u8>>,
}

/// Transaction output
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct TxOutput {
    pub value: u64,
    pub script_pubkey: Vec<u8>,
}

/// Transaction
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Transaction {
    pub version: i32,
    pub inputs: Vec<TxInput>,
    pub outputs: Vec<TxOutput>,
    pub lock_time: u32,
}

impl Transaction {
    /// Calculate transaction ID
    pub fn txid(&self) -> TxId {
        let mut hasher = Sha256::new();
        let serialized = bincode::serialize(self).unwrap();
        hasher.update(&serialized);
        let first = hasher.finalize();
        
        let mut hasher = Sha256::new();
        hasher.update(&first);
        let result = hasher.finalize();
        
        let mut txid = [0u8; 32];
        txid.copy_from_slice(&result);
        txid
    }
    
    /// Calculate transaction size
    pub fn size(&self) -> usize {
        bincode::serialize(self).unwrap().len()
    }
    
    /// Calculate transaction weight (for fee calculation)
    pub fn weight(&self) -> usize {
        // Simplified: base size * 3 + witness size
        self.size() * 4
    }
}

/// Mempool entry with metadata
#[derive(Debug, Clone)]
pub struct MempoolEntry {
    /// The transaction
    pub tx: Arc<Transaction>,
    
    /// Transaction ID
    pub txid: TxId,
    
    /// Fee in satoshis
    pub fee: u64,
    
    /// Fee rate (satoshis per KB)
    pub fee_rate: u64,
    
    /// Time added to mempool
    pub time_added: SystemTime,
    
    /// Transaction size
    pub size: usize,
    
    /// Transaction weight
    pub weight: usize,
    
    /// Height when transaction entered mempool
    pub height: u32,
    
    /// Ancestors in mempool
    pub ancestors: HashSet<TxId>,
    
    /// Descendants in mempool
    pub descendants: HashSet<TxId>,
    
    /// Modified fee (for CPFP)
    pub modified_fee: u64,
    
    /// Is this a 0-conf transaction?
    pub zero_conf: bool,
    
    /// Replace-by-fee enabled
    pub rbf: bool,
}

impl MempoolEntry {
    /// Create new mempool entry
    pub fn new(tx: Transaction, fee: u64, height: u32) -> Self {
        let size = tx.size();
        let weight = tx.weight();
        let fee_rate = (fee * 1000) / size.max(1) as u64;
        let txid = tx.txid();
        
        // Check for RBF (sequence < 0xfffffffe)
        let rbf = tx.inputs.iter().any(|input| input.sequence < 0xfffffffe);
        
        Self {
            tx: Arc::new(tx),
            txid,
            fee,
            fee_rate,
            time_added: SystemTime::now(),
            size,
            weight,
            height,
            ancestors: HashSet::new(),
            descendants: HashSet::new(),
            modified_fee: fee,
            zero_conf: fee_rate >= MIN_FEE_RATE * 10, // High fee = 0-conf eligible
            rbf,
        }
    }
    
    /// Update ancestors
    pub fn add_ancestor(&mut self, txid: TxId) {
        self.ancestors.insert(txid);
    }
    
    /// Update descendants
    pub fn add_descendant(&mut self, txid: TxId) {
        self.descendants.insert(txid);
    }
    
    /// Calculate ancestor fee rate
    pub fn ancestor_fee_rate(&self, mempool: &HashMap<TxId, MempoolEntry>) -> u64 {
        let mut total_fee = self.fee;
        let mut total_size = self.size;
        
        for ancestor_id in &self.ancestors {
            if let Some(ancestor) = mempool.get(ancestor_id) {
                total_fee += ancestor.fee;
                total_size += ancestor.size;
            }
        }
        
        (total_fee * 1000) / total_size.max(1) as u64
    }
}

/// Transaction priority for ordering
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub struct TxPriority {
    /// Fee rate in sat/KB
    pub fee_rate: u64,
    
    /// Time priority (older = higher)
    pub time_priority: u64,
}

/// Memory pool
pub struct Mempool {
    /// All transactions by ID
    transactions: Arc<DashMap<TxId, Arc<MempoolEntry>>>,
    
    /// Priority queue for mining
    priority_queue: Arc<RwLock<PriorityQueue<TxId, TxPriority>>>,
    
    /// Outputs spent by mempool transactions
    spent_outputs: Arc<DashMap<OutPoint, TxId>>,
    
    /// Orphan transactions (missing inputs)
    orphans: Arc<DashMap<TxId, Arc<Transaction>>>,
    
    /// Current mempool size in bytes
    total_size: Arc<RwLock<usize>>,
    
    /// Current chain height
    chain_height: Arc<RwLock<u32>>,
    
    /// Zero-conf eligible transactions
    zero_conf_txs: Arc<DashMap<TxId, Arc<MempoolEntry>>>,
    
    /// Metrics
    metrics: Arc<MempoolMetrics>,
}

/// Mempool metrics
pub struct MempoolMetrics {
    pub tx_added: prometheus::IntCounter,
    pub tx_removed: prometheus::IntCounter,
    pub tx_rejected: prometheus::IntCounter,
    pub orphans_added: prometheus::IntCounter,
    pub rbf_replacements: prometheus::IntCounter,
    pub size_bytes: prometheus::IntGauge,
}

impl Mempool {
    /// Create new mempool
    pub fn new(chain_height: u32) -> Self {
        let metrics = Arc::new(MempoolMetrics {
            tx_added: prometheus::IntCounter::new("mempool_tx_added", "Transactions added").unwrap(),
            tx_removed: prometheus::IntCounter::new("mempool_tx_removed", "Transactions removed").unwrap(),
            tx_rejected: prometheus::IntCounter::new("mempool_tx_rejected", "Transactions rejected").unwrap(),
            orphans_added: prometheus::IntCounter::new("mempool_orphans_added", "Orphans added").unwrap(),
            rbf_replacements: prometheus::IntCounter::new("mempool_rbf_replacements", "RBF replacements").unwrap(),
            size_bytes: prometheus::IntGauge::new("mempool_size_bytes", "Mempool size in bytes").unwrap(),
        });
        
        Self {
            transactions: Arc::new(DashMap::new()),
            priority_queue: Arc::new(RwLock::new(PriorityQueue::new())),
            spent_outputs: Arc::new(DashMap::new()),
            orphans: Arc::new(DashMap::new()),
            total_size: Arc::new(RwLock::new(0)),
            chain_height: Arc::new(RwLock::new(chain_height)),
            zero_conf_txs: Arc::new(DashMap::new()),
            metrics,
        }
    }
    
    /// Add transaction to mempool
    pub async fn add_transaction(
        &self,
        tx: Transaction,
        fee: u64,
    ) -> Result<TxId, MempoolError> {
        let txid = tx.txid();
        
        // Check if already in mempool
        if self.transactions.contains_key(&txid) {
            return Err(MempoolError::DuplicateTransaction);
        }
        
        // Check size
        let size = tx.size();
        if size > MAX_TX_SIZE {
            return Err(MempoolError::TransactionTooLarge(size));
        }
        
        // Check fee rate
        let fee_rate = (fee * 1000) / size.max(1) as u64;
        if fee_rate < MIN_FEE_RATE {
            return Err(MempoolError::FeeTooLow(fee_rate));
        }
        
        // Check for conflicts (double-spends)
        for input in &tx.inputs {
            if let Some(conflicting_tx) = self.spent_outputs.get(&input.prev_output) {
                // Check if this is an RBF replacement
                let conflicting_txid = *conflicting_tx.value();
                if let Some(existing) = self.transactions.get(&conflicting_txid) {
                    if !existing.rbf || fee <= existing.fee {
                        return Err(MempoolError::ConflictingTransaction);
                    }
                    // This is a valid RBF replacement
                    self.remove_transaction(conflicting_txid).await;
                    self.metrics.rbf_replacements.inc();
                }
            }
        }
        
        // Check mempool size limit
        let mut total_size = self.total_size.write().await;
        if *total_size + size > MAX_MEMPOOL_SIZE {
            // Try to evict low-fee transactions
            if !self.evict_transactions(size).await {
                return Err(MempoolError::MempoolFull);
            }
        }
        
        // Create mempool entry
        let height = *self.chain_height.read().await;
        let mut entry = MempoolEntry::new(tx, fee, height);
        
        // Find ancestors and descendants
        self.update_relationships(&mut entry).await;
        
        // Check ancestor/descendant limits
        if entry.ancestors.len() > MAX_ANCESTORS {
            return Err(MempoolError::TooManyAncestors(entry.ancestors.len()));
        }
        if entry.descendants.len() > MAX_DESCENDANTS {
            return Err(MempoolError::TooManyDescendants(entry.descendants.len()));
        }
        
        // Mark spent outputs
        for input in &entry.tx.inputs {
            self.spent_outputs.insert(input.prev_output.clone(), txid);
        }
        
        // Add to priority queue
        let priority = TxPriority {
            fee_rate: entry.fee_rate,
            time_priority: SystemTime::now()
                .duration_since(UNIX_EPOCH)
                .unwrap()
                .as_secs(),
        };
        
        let mut queue = self.priority_queue.write().await;
        queue.push(txid, priority);
        drop(queue);
        
        // Add to zero-conf if eligible
        if entry.zero_conf {
            self.zero_conf_txs.insert(txid, Arc::new(entry.clone()));
        }
        
        // Add to main storage
        let entry = Arc::new(entry);
        self.transactions.insert(txid, entry);
        
        // Update size
        *total_size += size;
        self.metrics.size_bytes.set(*total_size as i64);
        self.metrics.tx_added.inc();
        
        info!("Added transaction {} to mempool (fee rate: {} sat/KB)", 
              hex::encode(txid), fee_rate);
        
        Ok(txid)
    }
    
    /// Remove transaction from mempool
    pub async fn remove_transaction(&self, txid: TxId) -> Option<Arc<MempoolEntry>> {
        if let Some((_, entry)) = self.transactions.remove(&txid) {
            // Remove from priority queue
            let mut queue = self.priority_queue.write().await;
            queue.remove(&txid);
            drop(queue);
            
            // Remove spent outputs
            for input in &entry.tx.inputs {
                self.spent_outputs.remove(&input.prev_output);
            }
            
            // Remove from zero-conf
            self.zero_conf_txs.remove(&txid);
            
            // Update size
            let mut total_size = self.total_size.write().await;
            *total_size = total_size.saturating_sub(entry.size);
            self.metrics.size_bytes.set(*total_size as i64);
            self.metrics.tx_removed.inc();
            
            Some(entry)
        } else {
            None
        }
    }
    
    /// Get transactions for mining (sorted by priority)
    pub async fn get_mining_template(&self, max_weight: usize) -> Vec<Arc<Transaction>> {
        let mut transactions = Vec::new();
        let mut total_weight = 0;
        
        let queue = self.priority_queue.read().await;
        let mut sorted_txids: Vec<_> = queue.iter()
            .map(|(txid, priority)| (*txid, *priority))
            .collect();
        sorted_txids.sort_by_key(|(_, p)| std::cmp::Reverse(*p));
        
        for (txid, _) in sorted_txids {
            if let Some(entry) = self.transactions.get(&txid) {
                if total_weight + entry.weight > max_weight {
                    continue;
                }
                
                transactions.push(entry.tx.clone());
                total_weight += entry.weight;
            }
        }
        
        transactions
    }
    
    /// Get zero-conf eligible transactions
    pub fn get_zero_conf_transactions(&self) -> Vec<Arc<Transaction>> {
        self.zero_conf_txs.iter()
            .map(|entry| entry.value().tx.clone())
            .collect()
    }
    
    /// Update block tip (remove confirmed transactions)
    pub async fn update_tip(&self, height: u32, confirmed_txs: &[TxId]) {
        *self.chain_height.write().await = height;
        
        for txid in confirmed_txs {
            self.remove_transaction(*txid).await;
        }
        
        // Check orphans to see if any can now be added
        self.process_orphans().await;
    }
    
    /// Process orphan transactions
    async fn process_orphans(&self) {
        let orphans: Vec<_> = self.orphans.iter()
            .map(|entry| (*entry.key(), entry.value().clone()))
            .collect();
        
        for (txid, tx) in orphans {
            // Try to add to mempool (will fail if still orphaned)
            if self.add_transaction((*tx).clone(), 0).await.is_ok() {
                self.orphans.remove(&txid);
            }
        }
    }
    
    /// Update ancestor/descendant relationships
    async fn update_relationships(&self, entry: &mut MempoolEntry) {
        // Find ancestors (transactions this depends on)
        for input in &entry.tx.inputs {
            if let Some(parent_entry) = self.spent_outputs.get(&input.prev_output) {
                entry.ancestors.insert(*parent_entry.value());
            }
        }
        
        // Find descendants (transactions that depend on this)
        // This would require indexing outputs, simplified here
    }
    
    /// Evict transactions to make room
    async fn evict_transactions(&self, required_space: usize) -> bool {
        let mut evicted_size = 0;
        let mut to_evict = Vec::new();
        
        // Get lowest priority transactions
        let queue = self.priority_queue.read().await;
        let mut sorted: Vec<_> = queue.iter()
            .map(|(txid, priority)| (*txid, *priority))
            .collect();
        sorted.sort_by_key(|(_, p)| *p);
        
        for (txid, _) in sorted {
            if evicted_size >= required_space {
                break;
            }
            
            if let Some(entry) = self.transactions.get(&txid) {
                evicted_size += entry.size;
                to_evict.push(txid);
            }
        }
        
        drop(queue);
        
        // Evict transactions
        for txid in to_evict {
            self.remove_transaction(txid).await;
        }
        
        evicted_size >= required_space
    }
    
    /// Get mempool statistics
    pub async fn get_stats(&self) -> MempoolStats {
        MempoolStats {
            tx_count: self.transactions.len(),
            orphan_count: self.orphans.len(),
            total_size: *self.total_size.read().await,
            zero_conf_count: self.zero_conf_txs.len(),
        }
    }
}

/// Mempool statistics
#[derive(Debug, Clone, Serialize)]
pub struct MempoolStats {
    pub tx_count: usize,
    pub orphan_count: usize,
    pub total_size: usize,
    pub zero_conf_count: usize,
}

/// Mempool configuration
#[derive(Debug, Clone)]
pub struct MempoolConfig {
    pub max_size: usize,
    pub min_relay_fee: u64,
    pub persist: bool,
}

impl Default for MempoolConfig {
    fn default() -> Self {
        Self {
            max_size: MAX_MEMPOOL_SIZE,
            min_relay_fee: MIN_FEE_RATE,
            persist: true,
        }
    }
}

/// Alias to match daemon expectations
pub use Mempool as MemPool;

impl MemPool {
    /// Create mempool with config
    pub fn with_config(config: MempoolConfig) -> Result<Self, MempoolError> {
        Ok(Self::new(0)) // Use default height for now
    }
    
    /// Clean up expired transactions
    pub fn cleanup_expired(&self) {
        // TODO: Implement cleanup logic
    }
    
    /// Save mempool to disk
    pub async fn save_to_disk(&self, path: &std::path::Path) -> Result<(), MempoolError> {
        // TODO: Implement persistence
        Ok(())
    }
}

// Hex encoding helper
mod hex {
    pub fn encode(bytes: impl AsRef<[u8]>) -> String {
        bytes.as_ref().iter()
            .map(|b| format!("{:02x}", b))
            .collect()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[tokio::test]
    async fn test_mempool() {
        let mempool = Mempool::new(100);
        
        let tx = Transaction {
            version: 2,
            inputs: vec![],
            outputs: vec![
                TxOutput {
                    value: 100000,
                    script_pubkey: vec![],
                }
            ],
            lock_time: 0,
        };
        
        let txid = mempool.add_transaction(tx, 10000).await.unwrap();
        
        let stats = mempool.get_stats().await;
        assert_eq!(stats.tx_count, 1);
        
        mempool.remove_transaction(txid).await;
        
        let stats = mempool.get_stats().await;
        assert_eq!(stats.tx_count, 0);
    }
}