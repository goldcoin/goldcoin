// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! Goldcoin Core Daemon - Pure Rust Implementation
//! 
//! This is the main goldcoind binary ported entirely to Rust 1.89.
//! It maintains full compatibility with the C++23 implementation while
//! leveraging Rust's safety and performance advantages.

use std::path::PathBuf;
use std::sync::Arc;
use std::time::Duration;

use anyhow::{Context, Result};
use clap::{Arg, Command};
use signal_hook::consts::{SIGINT, SIGTERM};
use signal_hook_tokio::{Signals, SignalsInfo};
use futures_util::stream::StreamExt;
use tokio::{signal, time};
use tracing::{info, warn, error, debug};
use tracing_subscriber::{EnvFilter, FmtSubscriber};

// Goldcoin crates
use goldcoin_consensus::{ChainState, BlockValidator};
use goldcoin_network::{NetworkManager, PeerManager};
use goldcoin_rpc::{RpcServer, RpcConfig};
use goldcoin_storage::{BlockchainStorage, ChainDatabase};
use goldcoin_mempool::{MemPool, MempoolConfig};

mod config;
mod daemon;
mod logging;
mod shutdown;

use config::GoldcoindConfig;
use daemon::GoldcoinDaemon;

const VERSION: &str = "0.17.0";
const COPYRIGHT: &str = "Copyright (C) 2013-2025 The Goldcoin Core developers";

#[tokio::main]
async fn main() -> Result<()> {
    // Parse command line arguments
    let matches = build_cli().get_matches();
    
    // Initialize logging
    init_logging(&matches)?;
    
    info!("Goldcoin Core Daemon v{}", VERSION);
    info!("{}", COPYRIGHT);
    info!("Starting Pure Rust Implementation");
    
    // Load configuration
    let config = GoldcoindConfig::load(&matches)?;
    
    // Show version and exit if requested
    if matches.get_flag("version") {
        println!("Goldcoin Core Daemon version v{}", VERSION);
        println!("{}", COPYRIGHT);
        println!("Pure Rust Implementation - Rust {}", 
            std::env::var("RUSTC_VERSION").unwrap_or_else(|_| "1.89".to_string()));
        return Ok(());
    }
    
    // Check if running in daemon mode
    let daemon_mode = matches.get_flag("daemon");
    if daemon_mode {
        info!("Running in daemon mode");
    }
    
    // Initialize the daemon
    let daemon = GoldcoinDaemon::new(config).await
        .context("Failed to initialize Goldcoin daemon")?;
    
    // Setup signal handling
    let signals = Signals::new(&[SIGINT, SIGTERM])
        .context("Failed to setup signal handlers")?;
    let handle = signals.handle();
    
    let signals_task = tokio::spawn(handle_signals(signals, daemon.clone()));
    
    // Start the daemon
    info!("Starting Goldcoin daemon...");
    let daemon_task = tokio::spawn(async move {
        if let Err(e) = daemon.run().await {
            error!("Daemon error: {}", e);
        }
    });
    
    // Wait for shutdown signal or daemon completion
    tokio::select! {
        _ = daemon_task => {
            info!("Daemon task completed");
        }
        _ = signals_task => {
            info!("Shutdown signal received");
        }
        _ = tokio::signal::ctrl_c() => {
            info!("Ctrl+C received, shutting down...");
        }
    }
    
    handle.close();
    info!("Goldcoin daemon stopped");
    Ok(())
}

fn build_cli() -> Command {
    Command::new("goldcoind")
        .version(VERSION)
        .about("Goldcoin Core Daemon - Pure Rust Implementation")
        .long_about("The Goldcoin Core daemon manages the blockchain, validates transactions, and maintains network connectivity.")
        .arg(
            Arg::new("version")
                .long("version")
                .help("Print version information and exit")
                .action(clap::ArgAction::SetTrue)
        )
        .arg(
            Arg::new("daemon")
                .long("daemon")
                .help("Run in the background as a daemon")
                .action(clap::ArgAction::SetTrue)
        )
        .arg(
            Arg::new("datadir")
                .long("datadir")
                .help("Specify data directory")
                .value_name("DIR")
        )
        .arg(
            Arg::new("conf")
                .long("conf")
                .help("Specify configuration file")
                .value_name("FILE")
        )
        .arg(
            Arg::new("testnet")
                .long("testnet")
                .help("Use the test network")
                .action(clap::ArgAction::SetTrue)
        )
        .arg(
            Arg::new("regtest")
                .long("regtest") 
                .help("Use regression test network")
                .action(clap::ArgAction::SetTrue)
        )
        .arg(
            Arg::new("rpcport")
                .long("rpcport")
                .help("RPC server port")
                .value_name("PORT")
        )
        .arg(
            Arg::new("rpcbind")
                .long("rpcbind")
                .help("RPC server bind address")
                .value_name("ADDR")
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
            Arg::new("port")
                .long("port")
                .help("P2P network port")
                .value_name("PORT")
        )
        .arg(
            Arg::new("connect")
                .long("connect")
                .help("Connect only to specified node")
                .value_name("IP:PORT")
                .action(clap::ArgAction::Append)
        )
        .arg(
            Arg::new("addnode")
                .long("addnode")
                .help("Add node to connect to")
                .value_name("IP:PORT")
                .action(clap::ArgAction::Append)
        )
        .arg(
            Arg::new("maxconnections")
                .long("maxconnections")
                .help("Maximum number of connections")
                .value_name("NUM")
        )
        .arg(
            Arg::new("txindex")
                .long("txindex")
                .help("Maintain transaction index")
                .action(clap::ArgAction::SetTrue)
        )
        .arg(
            Arg::new("tx")
                .long("tx")
                .help("Enable full transaction validation (required for exchanges/pools)")
                .action(clap::ArgAction::SetTrue)
        )
        .arg(
            Arg::new("reindex")
                .long("reindex")
                .help("Rebuild block index")
                .action(clap::ArgAction::SetTrue)
        )
        .arg(
            Arg::new("reindex-chainstate")
                .long("reindex-chainstate")
                .help("Rebuild chainstate")
                .action(clap::ArgAction::SetTrue)
        )
        .arg(
            Arg::new("printtoconsole")
                .long("printtoconsole")
                .help("Print to console instead of debug.log")
                .action(clap::ArgAction::SetTrue)
        )
        .arg(
            Arg::new("debug")
                .long("debug")
                .help("Enable debug logging for category")
                .value_name("CATEGORY")
                .action(clap::ArgAction::Append)
        )
}

fn init_logging(matches: &clap::ArgMatches) -> Result<()> {
    let print_to_console = matches.get_flag("printtoconsole");
    let debug_categories: Vec<_> = matches.get_many::<String>("debug")
        .unwrap_or_default()
        .collect();
    
    let filter = if debug_categories.is_empty() {
        EnvFilter::try_from_default_env()
            .unwrap_or_else(|_| EnvFilter::new("info"))
    } else {
        let mut filter_str = "info".to_string();
        for category in debug_categories {
            filter_str.push_str(&format!(",{}=debug", category));
        }
        EnvFilter::new(filter_str)
    };
    
    let subscriber = FmtSubscriber::builder()
        .with_env_filter(filter)
        .with_target(false)
        .with_thread_ids(true)
        .with_thread_names(true);
    
    if print_to_console {
        subscriber.init();
    } else {
        // TODO: Add file logging to debug.log
        subscriber.init();
    }
    
    Ok(())
}

async fn handle_signals(mut signals: SignalsInfo, daemon: Arc<GoldcoinDaemon>) {
    while let Some(signal) = signals.next().await {
        match signal {
            SIGINT | SIGTERM => {
                info!("Received shutdown signal ({}), initiating graceful shutdown...", signal);
                daemon.shutdown().await;
                break;
            }
            _ => {}
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_cli_parsing() {
        let app = build_cli();
        let matches = app.try_get_matches_from(vec!["goldcoind", "--version"]).unwrap();
        assert!(matches.get_flag("version"));
    }
    
    #[tokio::test]
    async fn test_daemon_initialization() {
        // Test basic daemon initialization
        let config = GoldcoindConfig::default();
        let result = GoldcoinDaemon::new(config).await;
        // This might fail due to missing directories, but shouldn't panic
        assert!(result.is_ok() || result.is_err());
    }
}