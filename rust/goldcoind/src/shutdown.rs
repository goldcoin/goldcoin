// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! Graceful shutdown utilities

use std::sync::Arc;
use tokio::sync::broadcast;
use tracing::info;

/// Shutdown coordinator for graceful daemon termination
pub struct ShutdownCoordinator {
    shutdown_tx: broadcast::Sender<()>,
}

impl ShutdownCoordinator {
    pub fn new() -> (Self, broadcast::Receiver<()>) {
        let (shutdown_tx, shutdown_rx) = broadcast::channel(1);
        let coordinator = Self { shutdown_tx };
        (coordinator, shutdown_rx)
    }
    
    pub async fn shutdown(&self) {
        info!("Initiating coordinated shutdown...");
        let _ = self.shutdown_tx.send(());
    }
}