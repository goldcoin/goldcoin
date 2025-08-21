// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! Goldcoin CLI - Pure Rust Implementation
//! 
//! Command-line interface for interacting with goldcoind daemon.
//! Maintains full compatibility with the C++23 implementation.

use std::net::SocketAddr;
use std::path::PathBuf;

use anyhow::{Context, Result};
use clap::{Arg, Command, ArgMatches};
use colored::*;
// use comfy_table::{Table, Cell, Color};
use serde_json::{Value, json};
use tokio;

mod client;
mod commands;
mod config;
mod output;

use client::RpcClient;
use config::CliConfig;
use output::OutputFormat;

const VERSION: &str = "0.17.0";
const COPYRIGHT: &str = "Copyright (C) 2013-2025 The Goldcoin Core developers";

#[tokio::main]
async fn main() -> Result<()> {
    let matches = build_cli().get_matches();
    
    // Show version if requested
    if matches.get_flag("version") {
        println!("Goldcoin Core CLI version v{}", VERSION);
        println!("{}", COPYRIGHT);
        println!("Pure Rust Implementation - Rust {}", 
            std::env::var("RUSTC_VERSION").unwrap_or_else(|_| "1.89".to_string()));
        return Ok(());
    }
    
    // Load configuration
    let config = CliConfig::from_matches(&matches)?;
    
    // Create RPC client
    let client = RpcClient::new(config.clone()).await
        .context("Failed to create RPC client")?;
    
    // Get the command to execute
    let command = matches.get_one::<String>("command");
    let args: Vec<&str> = matches.get_many::<String>("args")
        .unwrap_or_default()
        .map(|s| s.as_str())
        .collect();
    
    if let Some(cmd) = command {
        // Execute the command
        let result = execute_command(&client, cmd, &args).await?;
        
        // Format and display output
        let output_format = if matches.get_flag("json") {
            OutputFormat::Json
        } else {
            OutputFormat::Pretty
        };
        
        output::display_result(&result, output_format);
    } else {
        // Interactive mode (if no command specified)
        println!("Goldcoin CLI v{} - Interactive Mode", VERSION);
        println!("Type 'help' for available commands or 'quit' to exit");
        
        interactive_mode(&client).await?;
    }
    
    Ok(())
}

fn build_cli() -> Command {
    Command::new("goldcoin-cli")
        .version(VERSION)
        .about("Goldcoin Core CLI - Pure Rust Implementation")
        .long_about("Command-line interface for interacting with goldcoind daemon")
        .arg(
            Arg::new("version")
                .long("version")
                .help("Print version information and exit")
                .action(clap::ArgAction::SetTrue)
        )
        .arg(
            Arg::new("conf")
                .long("conf")
                .help("Specify configuration file")
                .value_name("FILE")
        )
        .arg(
            Arg::new("datadir")
                .long("datadir")
                .help("Specify data directory")
                .value_name("DIR")
        )
        .arg(
            Arg::new("rpcconnect")
                .long("rpcconnect")
                .help("RPC server IP address")
                .value_name("IP")
                .default_value("127.0.0.1")
        )
        .arg(
            Arg::new("rpcport")
                .long("rpcport")
                .help("RPC server port")
                .value_name("PORT")
        )
        .arg(
            Arg::new("rpcuser")
                .long("rpcuser")
                .help("RPC username")
                .value_name("USER")
        )
        .arg(
            Arg::new("rpcpassword")
                .long("rpcpassword")
                .help("RPC password")
                .value_name("PASSWORD")
        )
        .arg(
            Arg::new("testnet")
                .long("testnet")
                .help("Use testnet")
                .action(clap::ArgAction::SetTrue)
        )
        .arg(
            Arg::new("regtest")
                .long("regtest")
                .help("Use regtest")
                .action(clap::ArgAction::SetTrue)
        )
        .arg(
            Arg::new("json")
                .long("json")
                .help("Output in JSON format")
                .action(clap::ArgAction::SetTrue)
        )
        .arg(
            Arg::new("timeout")
                .long("timeout")
                .help("Request timeout in seconds")
                .value_name("SECONDS")
                .default_value("30")
        )
        .arg(
            Arg::new("command")
                .help("RPC command to execute")
                .value_name("COMMAND")
                .index(1)
        )
        .arg(
            Arg::new("args")
                .help("Command arguments")
                .value_name("ARGS")
                .action(clap::ArgAction::Append)
                .index(2)
        )
}

async fn execute_command(client: &RpcClient, command: &str, args: &[&str]) -> Result<Value> {
    match command {
        // Blockchain commands
        "getblockchaininfo" => client.get_blockchain_info().await,
        "getbestblockhash" => client.get_best_block_hash().await,
        "getblockcount" => client.get_block_count().await,
        "getdifficulty" => client.get_difficulty().await,
        "getblock" => {
            let hash = args.get(0).context("Missing block hash")?;
            let verbosity = args.get(1).and_then(|v| v.parse().ok()).unwrap_or(1);
            client.get_block(hash, verbosity).await
        },
        "getblockheader" => {
            let hash = args.get(0).context("Missing block hash")?;
            let verbose = args.get(1).and_then(|v| v.parse().ok()).unwrap_or(true);
            client.get_block_header(hash, verbose).await
        },
        
        // Network commands
        "getpeerinfo" => client.get_peer_info().await,
        "getnetworkinfo" => client.get_network_info().await,
        "getconnectioncount" => client.get_connection_count().await,
        "addnode" => {
            let node = args.get(0).context("Missing node address")?;
            let command = if args.len() > 1 { &args[1] } else { "add" };
            client.add_node(node, command).await
        },
        
        // Mempool commands
        "getmempoolinfo" => client.get_mempool_info().await,
        "getrawmempool" => {
            let verbose = args.get(0).and_then(|v| v.parse().ok()).unwrap_or(false);
            client.get_raw_mempool(verbose).await
        },
        
        // Mining commands
        "getmininginfo" => client.get_mining_info().await,
        "getblocktemplate" => {
            let params = if args.is_empty() {
                None
            } else {
                Some(json!({"rules": args}))
            };
            client.get_block_template(params).await
        },
        "submitblock" => {
            let hex_data = args.get(0).context("Missing block hex data")?;
            client.submit_block(hex_data).await
        },
        
        // Wallet commands (if wallet is enabled)
        "getbalance" => {
            let account = args.get(0).map(|s| s.to_string());
            client.get_balance(account).await
        },
        "getnewaddress" => {
            let label = args.get(0).map(|s| s.to_string());
            client.get_new_address(label).await
        },
        "sendtoaddress" => {
            let address = args.get(0).context("Missing address")?;
            let amount: f64 = args.get(1).context("Missing amount")?.parse()
                .context("Invalid amount")?;
            client.send_to_address(address, amount).await
        },
        "listtransactions" => {
            let count = args.get(0).and_then(|c| c.parse().ok());
            client.list_transactions(count).await
        },
        
        // Utility commands
        "help" => {
            if let Some(cmd) = args.get(0) {
                client.help(Some(cmd)).await
            } else {
                client.help(None).await
            }
        },
        "stop" => client.stop().await,
        "uptime" => client.uptime().await,
        
        // Custom Goldcoin commands
        "getdefenseinfo" => client.get_defense_info().await,
        "getgoldenriverinfo" => client.get_golden_river_info().await,
        
        _ => Err(anyhow::anyhow!("Unknown command: {}", command)),
    }
}

async fn interactive_mode(client: &RpcClient) -> Result<()> {
    use std::io::{self, Write};
    
    loop {
        print!("goldcoin-cli> ");
        io::stdout().flush()?;
        
        let mut input = String::new();
        io::stdin().read_line(&mut input)?;
        let input = input.trim();
        
        if input.is_empty() {
            continue;
        }
        
        if input == "quit" || input == "exit" {
            println!("Goodbye!");
            break;
        }
        
        let parts: Vec<&str> = input.split_whitespace().collect();
        if parts.is_empty() {
            continue;
        }
        
        let command = parts[0];
        let args = &parts[1..];
        
        match execute_command(client, command, args).await {
            Ok(result) => {
                output::display_result(&result, OutputFormat::Pretty);
            }
            Err(e) => {
                eprintln!("{}: {}", "Error".red(), e);
            }
        }
    }
    
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_cli_parsing() {
        let app = build_cli();
        let matches = app.try_get_matches_from(vec!["goldcoin-cli", "getblockchaininfo"]).unwrap();
        assert_eq!(matches.get_one::<String>("command").unwrap(), "getblockchaininfo");
    }
    
    #[test]
    fn test_version_flag() {
        let app = build_cli();
        let matches = app.try_get_matches_from(vec!["goldcoin-cli", "--version"]).unwrap();
        assert!(matches.get_flag("version"));
    }
}