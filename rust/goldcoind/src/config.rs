// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! Configuration management for goldcoind

use std::net::SocketAddr;
use std::path::PathBuf;
use std::str::FromStr;

use anyhow::{Context, Result};
use clap::ArgMatches;
use serde::{Deserialize, Serialize};

/// Main daemon configuration
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct GoldcoindConfig {
    /// Network configuration
    pub network: NetworkConfig,
    
    /// RPC server configuration
    pub rpc: RpcConfig,
    
    /// Storage configuration
    pub storage: StorageConfig,
    
    /// Mempool configuration
    pub mempool: MempoolConfig,
    
    /// Mining configuration
    pub mining: MiningConfig,
    
    /// Logging configuration
    pub logging: LoggingConfig,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct NetworkConfig {
    /// P2P network port
    pub port: u16,
    
    /// Maximum number of peer connections
    pub max_connections: usize,
    
    /// Nodes to connect to
    pub connect_nodes: Vec<SocketAddr>,
    
    /// Additional nodes to try
    pub add_nodes: Vec<SocketAddr>,
    
    /// Network type (mainnet, testnet, regtest)
    pub network_type: NetworkType,
    
    /// Enable listening for incoming connections
    pub listen: bool,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct RpcConfig {
    /// RPC server bind address
    pub bind_addr: SocketAddr,
    
    /// RPC username
    pub user: String,
    
    /// RPC password
    pub password: String,
    
    /// Maximum RPC connections
    pub max_connections: usize,
    
    /// Enable RPC server
    pub enabled: bool,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct StorageConfig {
    /// Data directory
    pub data_dir: PathBuf,
    
    /// Transaction index enabled (full validation for exchanges/pools)
    pub tx_index: bool,
    
    /// Rebuild block index
    pub reindex: bool,
    
    /// Rebuild chainstate
    pub reindex_chainstate: bool,
    
    /// Database cache size (MB)
    pub db_cache: usize,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct MempoolConfig {
    /// Maximum mempool size (bytes)
    pub max_size: usize,
    
    /// Minimum relay fee (satoshis per KB)
    pub min_relay_fee: u64,
    
    /// Enable mempool persistence
    pub persist: bool,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct MiningConfig {
    /// Enable block generation
    pub enabled: bool,
    
    /// Mining address
    pub address: Option<String>,
    
    /// Block template cache duration (seconds)
    pub template_cache_duration: u64,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct LoggingConfig {
    /// Print to console instead of file
    pub print_to_console: bool,
    
    /// Debug categories
    pub debug_categories: Vec<String>,
    
    /// Log file path
    pub log_file: PathBuf,
}

#[derive(Debug, Clone, Copy, Serialize, Deserialize, PartialEq, Eq)]
pub enum NetworkType {
    Mainnet,
    Testnet,
    Regtest,
}

impl Default for GoldcoindConfig {
    fn default() -> Self {
        Self {
            network: NetworkConfig {
                port: 8121, // Goldcoin mainnet P2P port
                max_connections: 125,
                connect_nodes: Vec::new(),
                add_nodes: Vec::new(),
                network_type: NetworkType::Mainnet,
                listen: true,
            },
            rpc: RpcConfig {
                bind_addr: "127.0.0.1:8122".parse().unwrap(), // Goldcoin RPC port
                user: "goldcoinrpc".to_string(),
                password: String::new(),
                max_connections: 30,
                enabled: true,
            },
            storage: StorageConfig {
                data_dir: default_data_dir(),
                tx_index: false,
                reindex: false,
                reindex_chainstate: false,
                db_cache: 450, // MB
            },
            mempool: MempoolConfig {
                max_size: 300 * 1024 * 1024, // 300 MB
                min_relay_fee: 1000, // 1000 satoshis per KB
                persist: true,
            },
            mining: MiningConfig {
                enabled: false,
                address: None,
                template_cache_duration: 60,
            },
            logging: LoggingConfig {
                print_to_console: false,
                debug_categories: Vec::new(),
                log_file: default_data_dir().join("debug.log"),
            },
        }
    }
}

impl GoldcoindConfig {
    /// Load configuration from command line arguments and config file
    pub fn load(matches: &ArgMatches) -> Result<Self> {
        let mut config = Self::default();
        
        // Apply command line overrides
        config.apply_cli_args(matches)?;
        
        // Load config file if specified
        if let Some(conf_path) = matches.get_one::<String>("conf") {
            let conf_path = PathBuf::from(conf_path);
            if conf_path.exists() {
                config.load_config_file(&conf_path)?;
            }
        } else {
            // Try default config file
            let default_conf = config.storage.data_dir.join("goldcoin.conf");
            if default_conf.exists() {
                config.load_config_file(&default_conf)?;
            }
        }
        
        Ok(config)
    }
    
    /// Apply command line arguments to configuration
    fn apply_cli_args(&mut self, matches: &ArgMatches) -> Result<()> {
        // Data directory
        if let Some(datadir) = matches.get_one::<String>("datadir") {
            self.storage.data_dir = PathBuf::from(datadir);
            self.logging.log_file = self.storage.data_dir.join("debug.log");
        }
        
        // Network type
        if matches.get_flag("testnet") {
            self.network.network_type = NetworkType::Testnet;
            self.network.port = 18121; // Testnet port
            self.rpc.bind_addr = "127.0.0.1:18122".parse().unwrap();
        } else if matches.get_flag("regtest") {
            self.network.network_type = NetworkType::Regtest;
            self.network.port = 18121; // Regtest port  
            self.rpc.bind_addr = "127.0.0.1:18122".parse().unwrap();
        }
        
        // P2P port
        if let Some(port) = matches.get_one::<String>("port") {
            self.network.port = port.parse()
                .context("Invalid port number")?;
        }
        
        // RPC configuration
        if let Some(rpc_port) = matches.get_one::<String>("rpcport") {
            let port: u16 = rpc_port.parse()
                .context("Invalid RPC port number")?;
            self.rpc.bind_addr.set_port(port);
        }
        
        if let Some(rpc_bind) = matches.get_one::<String>("rpcbind") {
            self.rpc.bind_addr = rpc_bind.parse()
                .context("Invalid RPC bind address")?;
        }
        
        if let Some(rpc_user) = matches.get_one::<String>("rpcuser") {
            self.rpc.user = rpc_user.clone();
        }
        
        if let Some(rpc_password) = matches.get_one::<String>("rpcpassword") {
            self.rpc.password = rpc_password.clone();
        }
        
        // Connect nodes
        if let Some(connect_nodes) = matches.get_many::<String>("connect") {
            self.network.connect_nodes.clear();
            for node in connect_nodes {
                let addr = node.parse()
                    .context(format!("Invalid connect node address: {}", node))?;
                self.network.connect_nodes.push(addr);
            }
        }
        
        // Add nodes
        if let Some(add_nodes) = matches.get_many::<String>("addnode") {
            for node in add_nodes {
                let addr = node.parse()
                    .context(format!("Invalid addnode address: {}", node))?;
                self.network.add_nodes.push(addr);
            }
        }
        
        // Max connections
        if let Some(max_conn) = matches.get_one::<String>("maxconnections") {
            self.network.max_connections = max_conn.parse()
                .context("Invalid max connections number")?;
        }
        
        // Storage options
        if matches.get_flag("txindex") {
            self.storage.tx_index = true;
        }
        
        // Also support "tx" as an alias for txindex (undocumented but used)
        if matches.get_flag("tx") {
            self.storage.tx_index = true;
        }
        
        if matches.get_flag("reindex") {
            self.storage.reindex = true;
        }
        
        if matches.get_flag("reindex-chainstate") {
            self.storage.reindex_chainstate = true;
        }
        
        // Logging options
        if matches.get_flag("printtoconsole") {
            self.logging.print_to_console = true;
        }
        
        if let Some(debug_cats) = matches.get_many::<String>("debug") {
            self.logging.debug_categories = debug_cats.cloned().collect();
        }
        
        Ok(())
    }
    
    /// Load configuration from file
    fn load_config_file(&mut self, path: &PathBuf) -> Result<()> {
        let content = std::fs::read_to_string(path)
            .context(format!("Failed to read config file: {:?}", path))?;
        
        // Parse simple key=value format (Bitcoin-style config)
        for line in content.lines() {
            let line = line.trim();
            if line.is_empty() || line.starts_with('#') {
                continue;
            }
            
            if let Some((key, value)) = line.split_once('=') {
                self.apply_config_option(key.trim(), value.trim())?;
            }
        }
        
        Ok(())
    }
    
    /// Apply a single configuration option
    fn apply_config_option(&mut self, key: &str, value: &str) -> Result<()> {
        match key {
            "port" => self.network.port = value.parse()?,
            "rpcport" => self.rpc.bind_addr.set_port(value.parse()?),
            "rpcuser" => self.rpc.user = value.to_string(),
            "rpcpassword" => self.rpc.password = value.to_string(),
            "maxconnections" => self.network.max_connections = value.parse()?,
            "txindex" => self.storage.tx_index = value.parse().unwrap_or(true),
            "tx" => self.storage.tx_index = value.parse().unwrap_or(true), // Alias for txindex
            "dbcache" => self.storage.db_cache = value.parse()?,
            _ => {} // Ignore unknown options
        }
        Ok(())
    }
}

/// Get default data directory based on OS
fn default_data_dir() -> PathBuf {
    #[cfg(target_os = "windows")]
    {
        dirs::data_dir()
            .unwrap_or_else(|| PathBuf::from("."))
            .join("Goldcoin")
    }
    
    #[cfg(target_os = "macos")]
    {
        dirs::home_dir()
            .unwrap_or_else(|| PathBuf::from("."))
            .join("Library/Application Support/Goldcoin")
    }
    
    #[cfg(not(any(target_os = "windows", target_os = "macos")))]
    {
        dirs::home_dir()
            .unwrap_or_else(|| PathBuf::from("."))
            .join(".goldcoin")
    }
}

impl FromStr for NetworkType {
    type Err = anyhow::Error;
    
    fn from_str(s: &str) -> Result<Self> {
        match s.to_lowercase().as_str() {
            "mainnet" | "main" => Ok(NetworkType::Mainnet),
            "testnet" | "test" => Ok(NetworkType::Testnet),
            "regtest" | "reg" => Ok(NetworkType::Regtest),
            _ => Err(anyhow::anyhow!("Invalid network type: {}", s)),
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_default_config() {
        let config = GoldcoindConfig::default();
        assert_eq!(config.network.port, 51241);
        assert_eq!(config.rpc.bind_addr.port(), 8122);
        assert_eq!(config.network.network_type, NetworkType::Mainnet);
    }
    
    #[test]
    fn test_network_type_parsing() {
        assert_eq!("mainnet".parse::<NetworkType>().unwrap(), NetworkType::Mainnet);
        assert_eq!("testnet".parse::<NetworkType>().unwrap(), NetworkType::Testnet);
        assert_eq!("regtest".parse::<NetworkType>().unwrap(), NetworkType::Regtest);
    }
}