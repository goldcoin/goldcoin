// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! DNS seed discovery for Goldcoin network
//! 
//! This module provides DNS-based peer discovery using the official
//! Goldcoin DNS seed nodes.

use std::net::{IpAddr, SocketAddr, ToSocketAddrs};
use std::time::Duration;
use tracing::{info, warn, error, debug};

/// Official Goldcoin DNS seed nodes
pub const DNS_SEEDS: &[&str] = &[
    "seed.microguy.net",
    "cm.goldcoinproject.org", 
    "dnsseed.netseed.net",
    "seed.glcseed.net",
    "vps.netseed.net",
];

/// Goldcoin network magic bytes for mainnet
pub const NETWORK_MAGIC: [u8; 4] = [0xfd, 0xc2, 0xb4, 0xdd];

/// Default P2P port for mainnet
pub const DEFAULT_PORT: u16 = 8121;

/// Default RPC port for mainnet
pub const RPC_PORT: u16 = 8122;

/// Testnet P2P port
pub const TESTNET_PORT: u16 = 18121;

/// Testnet RPC port
pub const TESTNET_RPC_PORT: u16 = 18130;

/// Discover peers from DNS seeds
pub async fn discover_peers(max_peers: usize) -> Vec<SocketAddr> {
    let mut peers = Vec::new();
    
    for seed in DNS_SEEDS {
        info!("Querying DNS seed: {}", seed);
        
        match tokio::time::timeout(
            Duration::from_secs(5),
            resolve_seed(seed)
        ).await {
            Ok(Ok(addrs)) => {
                info!("DNS seed {} returned {} addresses", seed, addrs.len());
                for addr in addrs {
                    if peers.len() >= max_peers {
                        return peers;
                    }
                    peers.push(addr);
                    debug!("Added peer: {}", addr);
                }
            }
            Ok(Err(e)) => {
                warn!("Failed to resolve DNS seed {}: {}", seed, e);
            }
            Err(_) => {
                warn!("DNS seed {} timed out", seed);
            }
        }
    }
    
    info!("Discovered {} peers from DNS seeds", peers.len());
    peers
}

/// Resolve a DNS seed to socket addresses
async fn resolve_seed(seed: &str) -> std::io::Result<Vec<SocketAddr>> {
    let seed_with_port = format!("{}:{}", seed, DEFAULT_PORT);
    
    // Use tokio's DNS resolution
    let addrs = tokio::net::lookup_host(&seed_with_port).await?;
    Ok(addrs.collect())
}

/// Check if an address is valid for Goldcoin network
pub fn is_valid_peer_addr(addr: &SocketAddr) -> bool {
    match addr.ip() {
        IpAddr::V4(ip) => {
            // Filter out local and reserved addresses
            !ip.is_loopback() &&
            !ip.is_private() &&
            !ip.is_multicast() &&
            !ip.is_unspecified()
        }
        IpAddr::V6(ip) => {
            !ip.is_loopback() &&
            !ip.is_multicast() &&
            !ip.is_unspecified()
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_network_constants() {
        assert_eq!(NETWORK_MAGIC, [0xfd, 0xc2, 0xb4, 0xdd]);
        assert_eq!(DEFAULT_PORT, 8121);
        assert_eq!(RPC_PORT, 8122);
    }
    
    #[test]
    fn test_valid_peer_addr() {
        use std::net::Ipv4Addr;
        
        // Valid external address
        let addr = SocketAddr::new(IpAddr::V4(Ipv4Addr::new(8, 8, 8, 8)), 8121);
        assert!(is_valid_peer_addr(&addr));
        
        // Invalid loopback
        let addr = SocketAddr::new(IpAddr::V4(Ipv4Addr::new(127, 0, 0, 1)), 8121);
        assert!(!is_valid_peer_addr(&addr));
        
        // Invalid private
        let addr = SocketAddr::new(IpAddr::V4(Ipv4Addr::new(192, 168, 1, 1)), 8121);
        assert!(!is_valid_peer_addr(&addr));
    }
}