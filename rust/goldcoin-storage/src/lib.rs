//! Storage layer for Goldcoin blockchain data
//! 
//! Provides database abstraction and persistence for blocks, transactions, and UTXO set

use anyhow::Result;
use std::path::Path;
use std::sync::Arc;
use tokio::sync::RwLock;
use async_trait::async_trait;

/// Block information
#[derive(Debug, Clone)]
pub struct BlockInfo {
    pub hash: [u8; 32],
    pub height: u32,
    pub timestamp: u64,
}

/// Blockchain storage trait
#[async_trait]
pub trait BlockchainStorage: Send + Sync {
    async fn get_best_block(&self) -> Result<Option<BlockInfo>>;
    async fn flush(&self) -> Result<()>;
}

/// Chain database implementation
pub struct ChainDatabase {
    data_dir: std::path::PathBuf,
    cache_size: usize,
}

impl ChainDatabase {
    pub async fn new(data_dir: &Path, cache_size: usize) -> Result<Self> {
        Ok(Self {
            data_dir: data_dir.to_path_buf(),
            cache_size,
        })
    }
}

#[async_trait]
impl BlockchainStorage for ChainDatabase {
    async fn get_best_block(&self) -> Result<Option<BlockInfo>> {
        // TODO: Implement actual storage lookup
        Ok(None)
    }
    
    async fn flush(&self) -> Result<()> {
        Ok(())
    }
}

/// Database backend trait
pub trait Database: Send + Sync {
    /// Get value by key
    fn get(&self, key: &[u8]) -> Result<Option<Vec<u8>>>;
    
    /// Put key-value pair
    fn put(&self, key: &[u8], value: &[u8]) -> Result<()>;
    
    /// Delete key
    fn delete(&self, key: &[u8]) -> Result<()>;
    
    /// Check if key exists
    fn exists(&self, key: &[u8]) -> Result<bool>;
}

/// Block storage manager
pub struct BlockStore {
    db: Arc<RwLock<Box<dyn Database>>>,
}

impl BlockStore {
    /// Create new block store
    pub fn new(db: Box<dyn Database>) -> Self {
        BlockStore {
            db: Arc::new(RwLock::new(db)),
        }
    }

    /// Store a block
    pub async fn store_block(&self, height: u32, block_data: &[u8]) -> Result<()> {
        let key = format!("block:{}", height);
        let db = self.db.write().await;
        db.put(key.as_bytes(), block_data)
    }

    /// Retrieve a block
    pub async fn get_block(&self, height: u32) -> Result<Option<Vec<u8>>> {
        let key = format!("block:{}", height);
        let db = self.db.read().await;
        db.get(key.as_bytes())
    }

    /// Get best block height
    pub async fn get_best_height(&self) -> Result<Option<u32>> {
        let db = self.db.read().await;
        match db.get(b"best_height")? {
            Some(data) => {
                let height = u32::from_le_bytes(data.try_into().unwrap_or([0; 4]));
                Ok(Some(height))
            }
            None => Ok(None),
        }
    }

    /// Update best block height
    pub async fn set_best_height(&self, height: u32) -> Result<()> {
        let db = self.db.write().await;
        db.put(b"best_height", &height.to_le_bytes())
    }
}

/// UTXO set storage
pub struct UtxoStore {
    db: Arc<RwLock<Box<dyn Database>>>,
}

impl UtxoStore {
    /// Create new UTXO store
    pub fn new(db: Box<dyn Database>) -> Self {
        UtxoStore {
            db: Arc::new(RwLock::new(db)),
        }
    }

    /// Add UTXO to set
    pub async fn add_utxo(&self, txid: &[u8; 32], vout: u32, utxo_data: &[u8]) -> Result<()> {
        let key = Self::make_key(txid, vout);
        let db = self.db.write().await;
        db.put(&key, utxo_data)
    }

    /// Remove UTXO from set
    pub async fn remove_utxo(&self, txid: &[u8; 32], vout: u32) -> Result<()> {
        let key = Self::make_key(txid, vout);
        let db = self.db.write().await;
        db.delete(&key)
    }

    /// Get UTXO data
    pub async fn get_utxo(&self, txid: &[u8; 32], vout: u32) -> Result<Option<Vec<u8>>> {
        let key = Self::make_key(txid, vout);
        let db = self.db.read().await;
        db.get(&key)
    }

    /// Check if UTXO exists
    pub async fn has_utxo(&self, txid: &[u8; 32], vout: u32) -> Result<bool> {
        let key = Self::make_key(txid, vout);
        let db = self.db.read().await;
        db.exists(&key)
    }

    fn make_key(txid: &[u8; 32], vout: u32) -> Vec<u8> {
        let mut key = Vec::with_capacity(36);
        key.extend_from_slice(txid);
        key.extend_from_slice(&vout.to_le_bytes());
        key
    }
}

/// In-memory database for testing
pub struct MemoryDatabase {
    data: std::collections::HashMap<Vec<u8>, Vec<u8>>,
}

impl MemoryDatabase {
    pub fn new() -> Self {
        MemoryDatabase {
            data: std::collections::HashMap::new(),
        }
    }
}

impl Database for MemoryDatabase {
    fn get(&self, key: &[u8]) -> Result<Option<Vec<u8>>> {
        Ok(self.data.get(key).cloned())
    }

    fn put(&self, key: &[u8], value: &[u8]) -> Result<()> {
        // Note: This would need to be mutable in real implementation
        Ok(())
    }

    fn delete(&self, key: &[u8]) -> Result<()> {
        // Note: This would need to be mutable in real implementation
        Ok(())
    }

    fn exists(&self, key: &[u8]) -> Result<bool> {
        Ok(self.data.contains_key(key))
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[tokio::test]
    async fn test_block_store() {
        let db = Box::new(MemoryDatabase::new());
        let store = BlockStore::new(db);
        
        // Test storing and retrieving blocks
        let block_data = b"test block data";
        store.store_block(1, block_data).await.unwrap();
        
        // Test best height
        store.set_best_height(100).await.unwrap();
        let height = store.get_best_height().await.unwrap();
        assert_eq!(height, Some(100));
    }
}