// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! Configuration for goldcoin-cli

use std::path::PathBuf;
use anyhow::{Context, Result};
use clap::ArgMatches;

#[derive(Debug, Clone)]
pub struct CliConfig {
    pub rpc_url: String,
    pub rpc_user: String,
    pub rpc_password: String,
    pub timeout: u64,
    pub data_dir: Option<PathBuf>,
}

impl CliConfig {
    pub fn from_matches(matches: &ArgMatches) -> Result<Self> {
        let mut config = Self::default();
        
        // Network type affects default port
        let default_port = if matches.get_flag("testnet") {
            8123 // Testnet RPC port
        } else if matches.get_flag("regtest") {
            8124 // Regtest RPC port
        } else {
            8122 // Mainnet RPC port
        };
        
        // Build RPC URL
        let host = matches.get_one::<String>("rpcconnect")
            .map(|s| s.as_str())
            .unwrap_or("127.0.0.1");
        
        let port = matches.get_one::<String>("rpcport")
            .and_then(|p| p.parse().ok())
            .unwrap_or(default_port);
        
        config.rpc_url = format!("http://{}:{}", host, port);
        
        // Authentication
        if let Some(user) = matches.get_one::<String>("rpcuser") {
            config.rpc_user = user.clone();
        }
        
        if let Some(password) = matches.get_one::<String>("rpcpassword") {
            config.rpc_password = password.clone();
        }
        
        // Timeout
        if let Some(timeout) = matches.get_one::<String>("timeout") {
            config.timeout = timeout.parse()
                .context("Invalid timeout value")?;
        }
        
        // Data directory
        if let Some(datadir) = matches.get_one::<String>("datadir") {
            config.data_dir = Some(PathBuf::from(datadir));
        }
        
        // Try to load config from file
        config.load_from_file()?;
        
        Ok(config)
    }
    
    fn load_from_file(&mut self) -> Result<()> {
        let config_path = if let Some(ref data_dir) = self.data_dir {
            data_dir.join("goldcoin.conf")
        } else {
            default_data_dir().join("goldcoin.conf")
        };
        
        if config_path.exists() {
            let content = std::fs::read_to_string(&config_path)
                .context("Failed to read config file")?;
            
            for line in content.lines() {
                let line = line.trim();
                if line.is_empty() || line.starts_with('#') {
                    continue;
                }
                
                if let Some((key, value)) = line.split_once('=') {
                    match key.trim() {
                        "rpcuser" if self.rpc_user.is_empty() => {
                            self.rpc_user = value.trim().to_string();
                        }
                        "rpcpassword" if self.rpc_password.is_empty() => {
                            self.rpc_password = value.trim().to_string();
                        }
                        _ => {}
                    }
                }
            }
        }
        
        Ok(())
    }
}

impl Default for CliConfig {
    fn default() -> Self {
        Self {
            rpc_url: "http://127.0.0.1:8122".to_string(),
            rpc_user: String::new(),
            rpc_password: String::new(),
            timeout: 30,
            data_dir: None,
        }
    }
}

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