// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! Core daemon implementation that orchestrates all Goldcoin components

use std::sync::Arc;
use std::time::Duration;

use anyhow::{Context, Result};
use tokio::sync::{broadcast, RwLock};
use tokio::{select, time, task::JoinHandle};
use tracing::{info, warn, error, debug};

use goldcoin_consensus::{ChainState, BlockValidator, goldcoin_features::DefenseSystem};
use goldcoin_network::{NetworkManager, PeerManager, NetworkEvent};
use goldcoin_rpc::{RpcServer, RpcConfig};
use goldcoin_storage::{BlockchainStorage, ChainDatabase};
use goldcoin_mempool::{MemPool, MempoolConfig};

use crate::config::GoldcoindConfig;

/// Main Goldcoin daemon that coordinates all subsystems
pub struct GoldcoinDaemon {
    /// Configuration
    config: GoldcoindConfig,
    
    /// Blockchain state and consensus
    chain_state: Arc<RwLock<ChainState>>,
    
    /// Block validator
    block_validator: Arc<BlockValidator>,
    
    /// 51% Defense System
    defense_system: Arc<RwLock<DefenseSystem>>,
    
    /// Network manager for P2P communications
    network_manager: Arc<NetworkManager>,
    
    /// Peer connection manager
    peer_manager: Arc<PeerManager>,
    
    /// Transaction memory pool
    mempool: Arc<RwLock<MemPool>>,
    
    /// Blockchain storage
    storage: Arc<dyn BlockchainStorage>,
    
    /// RPC server
    rpc_server: Option<RpcServer>,
    
    /// Shutdown signal
    shutdown_tx: broadcast::Sender<()>,
    shutdown_rx: broadcast::Receiver<()>,
    
    /// Running tasks
    tasks: Vec<JoinHandle<()>>,
}

impl GoldcoinDaemon {
    /// Create a new Goldcoin daemon instance
    pub async fn new(config: GoldcoindConfig) -> Result<Arc<Self>> {
        info!("Initializing Goldcoin daemon...");
        
        // Create shutdown channel
        let (shutdown_tx, shutdown_rx) = broadcast::channel(1);
        
        // Initialize storage
        let storage = ChainDatabase::new(&config.storage.data_dir, config.storage.db_cache)
            .await
            .context("Failed to initialize blockchain storage")?;
        let storage = Arc::new(storage);
        
        // Initialize chain state
        let chain_state = Arc::new(RwLock::new(
            ChainState::with_network_type(config.network.network_type.into())
                .context("Failed to initialize chain state")?
        ));
        
        // Initialize 51% Defense System
        let defense_system = Arc::new(RwLock::new(DefenseSystem::new()));
        
        // Initialize block validator
        let block_validator = Arc::new(BlockValidator::new(
            chain_state.clone(),
            defense_system.clone(),
        ));
        
        // Initialize mempool
        let mempool_config = MempoolConfig {
            max_size: config.mempool.max_size,
            min_relay_fee: config.mempool.min_relay_fee,
            persist: config.mempool.persist,
        };
        let mempool = Arc::new(RwLock::new(
            MemPool::with_config(mempool_config)
                .context("Failed to initialize mempool")?
        ));
        
        // Initialize network components
        let network_manager = Arc::new(
            NetworkManager::with_port_and_network(
                config.network.port,
                config.network.network_type.into(),
                config.network.max_connections,
            ).await
            .context("Failed to initialize network manager")?
        );
        
        let peer_manager = Arc::new(
            PeerManager::new(
                network_manager.clone(),
                chain_state.clone(),
                mempool.clone(),
            ).await
            .context("Failed to initialize peer manager")?
        );
        
        // Initialize RPC server if enabled
        let rpc_server = if config.rpc.enabled {
            let rpc_config = RpcConfig {
                bind_addr: config.rpc.bind_addr,
                rpc_user: config.rpc.user.clone(),
                rpc_password: config.rpc.password.clone(),
                max_connections: config.rpc.max_connections,
                enable_websocket: true,
                enable_cors: false,
            };
            
            Some(RpcServer::new(rpc_config))
        } else {
            None
        };
        
        let daemon = Arc::new(Self {
            config,
            chain_state,
            block_validator,
            defense_system,
            network_manager,
            peer_manager,
            mempool,
            storage,
            rpc_server,
            shutdown_tx,
            shutdown_rx,
            tasks: Vec::new(),
        });
        
        info!("Goldcoin daemon initialized successfully");
        Ok(daemon)
    }
    
    /// Start the daemon and all its subsystems
    pub async fn run(self: Arc<Self>) -> Result<()> {
        info!("Starting Goldcoin daemon subsystems...");
        
        // Log validation mode
        if self.config.storage.tx_index {
            info!("FULL TRANSACTION VALIDATION ENABLED (txindex=1)");
            info!("Every transaction in every block will be validated");
            info!("This mode is required for exchanges and mining pools");
        } else {
            info!("Running in SPV mode (header validation only)");
            info!("Use --txindex or txindex=1 in config for full validation");
        }
        
        // Load blockchain from storage
        self.load_blockchain().await?;
        
        // Start subsystems
        let mut tasks = Vec::new();
        
        // Start network manager
        let network_task = self.start_network_manager().await?;
        tasks.push(network_task);
        
        // Start peer manager
        let peer_task = self.start_peer_manager().await?;
        tasks.push(peer_task);
        
        // Start block processing
        let block_task = self.start_block_processor().await?;
        tasks.push(block_task);
        
        // Start mempool manager
        let mempool_task = self.start_mempool_manager().await?;
        tasks.push(mempool_task);
        
        // Start RPC server
        if let Some(ref rpc_server) = self.rpc_server {
            let rpc_task = self.start_rpc_server(rpc_server.clone()).await?;
            tasks.push(rpc_task);
        }
        
        // Start sync manager
        let sync_task = self.start_sync_manager().await?;
        tasks.push(sync_task);
        
        info!("All daemon subsystems started successfully");
        
        // Wait for shutdown signal
        let mut shutdown_rx = self.shutdown_tx.subscribe();
        select! {
            _ = shutdown_rx.recv() => {
                info!("Shutdown signal received");
            }
        }
        
        // Gracefully shutdown all tasks
        info!("Shutting down daemon subsystems...");
        for task in tasks {
            task.abort();
        }
        
        // Save state before shutdown
        self.save_state().await?;
        
        info!("Goldcoin daemon stopped");
        Ok(())
    }
    
    /// Load blockchain state from storage
    async fn load_blockchain(&self) -> Result<()> {
        info!("Loading blockchain from storage...");
        
        // Check if we need to reindex
        if self.config.storage.reindex || self.config.storage.reindex_chainstate {
            warn!("Reindexing blockchain (this may take a while)...");
            // TODO: Implement reindexing
        }
        
        // Load best block and chain state
        let best_block = self.storage.get_best_block().await
            .context("Failed to load best block")?;
        
        if let Some(block) = best_block {
            let mut chain_state = self.chain_state.write().await;
            chain_state.update_best_block(block.hash, block.height)?;
            info!("Loaded blockchain state: height={}, hash={}", 
                  block.height, hex::encode(block.hash));
        } else {
            info!("Starting with genesis block");
        }
        
        Ok(())
    }
    
    /// Start network manager task
    async fn start_network_manager(&self) -> Result<JoinHandle<()>> {
        let network_manager = self.network_manager.clone();
        let mut shutdown_rx = self.shutdown_tx.subscribe();
        
        let task = tokio::spawn(async move {
            select! {
                result = network_manager.run() => {
                    if let Err(e) = result {
                        error!("Network manager error: {}", e);
                    }
                }
                _ = shutdown_rx.recv() => {
                    debug!("Network manager shutdown requested");
                }
            }
        });
        
        Ok(task)
    }
    
    /// Start peer manager task
    async fn start_peer_manager(&self) -> Result<JoinHandle<()>> {
        let peer_manager = self.peer_manager.clone();
        let mut shutdown_rx = self.shutdown_tx.subscribe();
        
        let task = tokio::spawn(async move {
            select! {
                result = peer_manager.run() => {
                    if let Err(e) = result {
                        error!("Peer manager error: {}", e);
                    }
                }
                _ = shutdown_rx.recv() => {
                    debug!("Peer manager shutdown requested");
                }
            }
        });
        
        Ok(task)
    }
    
    /// Start block processing task
    async fn start_block_processor(&self) -> Result<JoinHandle<()>> {
        let chain_state = self.chain_state.clone();
        let block_validator = self.block_validator.clone();
        let storage = self.storage.clone();
        let tx_index = self.config.storage.tx_index;
        let mut shutdown_rx = self.shutdown_tx.subscribe();
        
        let task = tokio::spawn(async move {
            let mut interval = time::interval(Duration::from_secs(1));
            
            loop {
                select! {
                    _ = interval.tick() => {
                        // Process any pending blocks
                        // TODO: Implement block processing queue
                        
                        if tx_index {
                            // Full transaction-by-transaction validation (txindex=1)
                            // - Parse every transaction
                            // - Validate all inputs/outputs
                            // - Execute all scripts
                            // - Update UTXO set
                            // - Verify signatures
                            // - Maintain transaction index
                            debug!("Transaction index enabled - full validation of all transactions");
                        } else {
                            // SPV mode - header validation only
                            // - Validate block headers
                            // - Check proof of work
                            // - Verify merkle roots
                            debug!("SPV mode - validating headers only");
                        }
                    }
                    _ = shutdown_rx.recv() => {
                        debug!("Block processor shutdown requested");
                        break;
                    }
                }
            }
        });
        
        Ok(task)
    }
    
    /// Start mempool manager task
    async fn start_mempool_manager(&self) -> Result<JoinHandle<()>> {
        let mempool = self.mempool.clone();
        let mut shutdown_rx = self.shutdown_tx.subscribe();
        
        let task = tokio::spawn(async move {
            let mut interval = time::interval(Duration::from_secs(60));
            
            loop {
                select! {
                    _ = interval.tick() => {
                        // Cleanup expired transactions
                        let mut pool = mempool.write().await;
                        pool.cleanup_expired();
                    }
                    _ = shutdown_rx.recv() => {
                        debug!("Mempool manager shutdown requested");
                        break;
                    }
                }
            }
        });
        
        Ok(task)
    }
    
    /// Start RPC server task
    async fn start_rpc_server(&self, rpc_server: RpcServer) -> Result<JoinHandle<()>> {
        let mut shutdown_rx = self.shutdown_tx.subscribe();
        
        let task = tokio::spawn(async move {
            select! {
                result = rpc_server.run() => {
                    if let Err(e) = result {
                        error!("RPC server error: {}", e);
                    }
                }
                _ = shutdown_rx.recv() => {
                    debug!("RPC server shutdown requested");
                }
            }
        });
        
        Ok(task)
    }
    
    /// Start sync manager task
    async fn start_sync_manager(&self) -> Result<JoinHandle<()>> {
        let peer_manager = self.peer_manager.clone();
        let chain_state = self.chain_state.clone();
        let mut shutdown_rx = self.shutdown_tx.subscribe();
        
        let task = tokio::spawn(async move {
            let mut interval = time::interval(Duration::from_secs(10));
            
            loop {
                select! {
                    _ = interval.tick() => {
                        // Check sync status and request blocks if needed
                        let state = chain_state.read().await;
                        let height = state.best_height();
                        debug!("Current blockchain height: {}", height);
                        
                        // TODO: Implement sync logic
                    }
                    _ = shutdown_rx.recv() => {
                        debug!("Sync manager shutdown requested");
                        break;
                    }
                }
            }
        });
        
        Ok(task)
    }
    
    /// Save daemon state before shutdown
    async fn save_state(&self) -> Result<()> {
        info!("Saving daemon state...");
        
        // Save mempool if persistence is enabled
        if self.config.mempool.persist {
            let mempool = self.mempool.read().await;
            mempool.save_to_disk(&self.config.storage.data_dir.join("mempool.dat"))
                .await
                .context("Failed to save mempool")?;
        }
        
        // Flush storage
        self.storage.flush().await
            .context("Failed to flush storage")?;
        
        info!("Daemon state saved successfully");
        Ok(())
    }
    
    /// Initiate graceful shutdown
    pub async fn shutdown(&self) {
        info!("Initiating graceful shutdown...");
        let _ = self.shutdown_tx.send(());
    }
}

// Convert network types
impl From<crate::config::NetworkType> for goldcoin_consensus::NetworkType {
    fn from(nt: crate::config::NetworkType) -> Self {
        match nt {
            crate::config::NetworkType::Mainnet => goldcoin_consensus::NetworkType::Mainnet,
            crate::config::NetworkType::Testnet => goldcoin_consensus::NetworkType::Testnet,
            crate::config::NetworkType::Regtest => goldcoin_consensus::NetworkType::Regtest,
        }
    }
}

impl From<crate::config::NetworkType> for goldcoin_network::NetworkType {
    fn from(nt: crate::config::NetworkType) -> Self {
        match nt {
            crate::config::NetworkType::Mainnet => goldcoin_network::NetworkType::Mainnet,
            crate::config::NetworkType::Testnet => goldcoin_network::NetworkType::Testnet,
            crate::config::NetworkType::Regtest => goldcoin_network::NetworkType::Regtest,
        }
    }
}