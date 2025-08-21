// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! RPC client implementation for goldcoin-cli

use std::time::Duration;
use anyhow::{Context, Result};
use reqwest::{Client, RequestBuilder};
use serde_json::{Value, json};
use base64::prelude::*;

use crate::config::CliConfig;

/// JSON-RPC client for communicating with goldcoind
pub struct RpcClient {
    client: Client,
    config: CliConfig,
    id_counter: std::sync::atomic::AtomicU64,
}

impl RpcClient {
    /// Create a new RPC client
    pub async fn new(config: CliConfig) -> Result<Self> {
        let client = Client::builder()
            .timeout(Duration::from_secs(config.timeout))
            .build()
            .context("Failed to create HTTP client")?;
        
        let client = Self {
            client,
            config,
            id_counter: std::sync::atomic::AtomicU64::new(1),
        };
        
        // Test connection
        client.test_connection().await
            .context("Failed to connect to goldcoind")?;
        
        Ok(client)
    }
    
    /// Test connection to the daemon
    async fn test_connection(&self) -> Result<()> {
        match self.call_rpc("uptime", &[]).await {
            Ok(_) => Ok(()),
            Err(e) => {
                if e.to_string().contains("Method not found") {
                    // uptime might not be implemented, try getblockchaininfo
                    self.call_rpc("getblockchaininfo", &[]).await?;
                    Ok(())
                } else {
                    Err(e)
                }
            }
        }
    }
    
    /// Make a JSON-RPC call
    async fn call_rpc(&self, method: &str, params: &[Value]) -> Result<Value> {
        let id = self.id_counter.fetch_add(1, std::sync::atomic::Ordering::SeqCst);
        
        let request = json!({
            "jsonrpc": "2.0",
            "method": method,
            "params": params,
            "id": id
        });
        
        let mut req_builder = self.client
            .post(&self.config.rpc_url)
            .json(&request);
        
        // Add authentication if provided
        if !self.config.rpc_user.is_empty() {
            let auth = format!("{}:{}", self.config.rpc_user, self.config.rpc_password);
            let auth_header = format!("Basic {}", BASE64_STANDARD.encode(auth.as_bytes()));
            req_builder = req_builder.header("Authorization", auth_header);
        }
        
        let response = req_builder
            .send()
            .await
            .context("Failed to send RPC request")?;
        
        if !response.status().is_success() {
            return Err(anyhow::anyhow!("HTTP error: {}", response.status()));
        }
        
        let json_response: Value = response.json().await
            .context("Failed to parse JSON response")?;
        
        if let Some(error) = json_response.get("error") {
            if !error.is_null() {
                let error_message = error.get("message")
                    .and_then(|m| m.as_str())
                    .unwrap_or("Unknown error");
                let error_code = error.get("code")
                    .and_then(|c| c.as_i64())
                    .unwrap_or(-1);
                return Err(anyhow::anyhow!("error code: {}\nerror message:\n{}", error_code, error_message));
            }
        }
        
        json_response.get("result")
            .cloned()
            .ok_or_else(|| anyhow::anyhow!("Missing result in response"))
    }
    
    // Blockchain RPC methods
    pub async fn get_blockchain_info(&self) -> Result<Value> {
        self.call_rpc("getblockchaininfo", &[]).await
    }
    
    pub async fn get_best_block_hash(&self) -> Result<Value> {
        self.call_rpc("getbestblockhash", &[]).await
    }
    
    pub async fn get_block_count(&self) -> Result<Value> {
        self.call_rpc("getblockcount", &[]).await
    }
    
    pub async fn get_difficulty(&self) -> Result<Value> {
        self.call_rpc("getdifficulty", &[]).await
    }
    
    pub async fn get_block(&self, hash: &str, verbosity: u8) -> Result<Value> {
        self.call_rpc("getblock", &[json!(hash), json!(verbosity)]).await
    }
    
    pub async fn get_block_header(&self, hash: &str, verbose: bool) -> Result<Value> {
        self.call_rpc("getblockheader", &[json!(hash), json!(verbose)]).await
    }
    
    // Network RPC methods
    pub async fn get_peer_info(&self) -> Result<Value> {
        self.call_rpc("getpeerinfo", &[]).await
    }
    
    pub async fn get_network_info(&self) -> Result<Value> {
        self.call_rpc("getnetworkinfo", &[]).await
    }
    
    pub async fn get_connection_count(&self) -> Result<Value> {
        self.call_rpc("getconnectioncount", &[]).await
    }
    
    pub async fn add_node(&self, node: &str, command: &str) -> Result<Value> {
        self.call_rpc("addnode", &[json!(node), json!(command)]).await
    }
    
    // Mempool RPC methods
    pub async fn get_mempool_info(&self) -> Result<Value> {
        self.call_rpc("getmempoolinfo", &[]).await
    }
    
    pub async fn get_raw_mempool(&self, verbose: bool) -> Result<Value> {
        self.call_rpc("getrawmempool", &[json!(verbose)]).await
    }
    
    // Mining RPC methods
    pub async fn get_mining_info(&self) -> Result<Value> {
        self.call_rpc("getmininginfo", &[]).await
    }
    
    pub async fn get_block_template(&self, params: Option<Value>) -> Result<Value> {
        let args = if let Some(p) = params {
            vec![p]
        } else {
            vec![]
        };
        self.call_rpc("getblocktemplate", &args).await
    }
    
    pub async fn submit_block(&self, hex_data: &str) -> Result<Value> {
        self.call_rpc("submitblock", &[json!(hex_data)]).await
    }
    
    // Wallet RPC methods
    pub async fn get_balance(&self, account: Option<String>) -> Result<Value> {
        let args = if let Some(acc) = account {
            vec![json!(acc)]
        } else {
            vec![]
        };
        self.call_rpc("getbalance", &args).await
    }
    
    pub async fn get_new_address(&self, label: Option<String>) -> Result<Value> {
        let args = if let Some(lbl) = label {
            vec![json!(lbl)]
        } else {
            vec![]
        };
        self.call_rpc("getnewaddress", &args).await
    }
    
    pub async fn send_to_address(&self, address: &str, amount: f64) -> Result<Value> {
        self.call_rpc("sendtoaddress", &[json!(address), json!(amount)]).await
    }
    
    pub async fn list_transactions(&self, count: Option<u32>) -> Result<Value> {
        let args = if let Some(c) = count {
            vec![json!(c)]
        } else {
            vec![]
        };
        self.call_rpc("listtransactions", &args).await
    }
    
    // Utility methods
    pub async fn help(&self, command: Option<&str>) -> Result<Value> {
        let args = if let Some(cmd) = command {
            vec![json!(cmd)]
        } else {
            vec![]
        };
        self.call_rpc("help", &args).await
    }
    
    pub async fn stop(&self) -> Result<Value> {
        self.call_rpc("stop", &[]).await
    }
    
    pub async fn uptime(&self) -> Result<Value> {
        self.call_rpc("uptime", &[]).await
    }
    
    // Custom Goldcoin methods
    pub async fn get_defense_info(&self) -> Result<Value> {
        self.call_rpc("getdefenseinfo", &[]).await
    }
    
    pub async fn get_golden_river_info(&self) -> Result<Value> {
        self.call_rpc("getgoldenriverinfo", &[]).await
    }
}