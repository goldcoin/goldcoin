// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! FFI Bridge between C++23 and Rust
//! 
//! This module provides safe FFI bindings to integrate Rust networking
//! and consensus code with the existing C++23 codebase.

use std::ffi::{CStr, CString};
use std::os::raw::{c_char, c_void};
use std::ptr;
use std::slice;
use std::sync::Arc;

use goldcoin_network::{NetworkManager, NetworkConfig, NetworkEvent};
use goldcoin_consensus::{Block, Transaction, ChainState, ValidationError};
use goldcoin_consensus::goldcoin_features::{GoldenRiverDifficulty, DefenseSystem};

/// Opaque handle for network manager
pub struct NetworkHandle {
    manager: Arc<NetworkManager>,
    runtime: tokio::runtime::Runtime,
}

/// Opaque handle for chain state
pub struct ChainHandle {
    state: Arc<tokio::sync::RwLock<ChainState>>,
    runtime: tokio::runtime::Runtime,
}

/// FFI result type
#[repr(C)]
pub struct FFIResult {
    success: bool,
    error_msg: *const c_char,
}

impl FFIResult {
    fn ok() -> Self {
        Self {
            success: true,
            error_msg: ptr::null(),
        }
    }
    
    fn err(msg: String) -> Self {
        let c_string = CString::new(msg).unwrap();
        Self {
            success: false,
            error_msg: c_string.into_raw(),
        }
    }
}

/// Network configuration for FFI
#[repr(C)]
pub struct FFINetworkConfig {
    listen_port: u16,
    max_peers: usize,
    max_outbound: usize,
    protocol_version: u32,
    user_agent: *const c_char,
    relay: bool,
    services: u64,
}

/// Block header for FFI
#[repr(C)]
pub struct FFIBlockHeader {
    version: i32,
    prev_block: [u8; 32],
    merkle_root: [u8; 32],
    timestamp: u32,
    bits: u32,
    nonce: u32,
}

/// Transaction for FFI
#[repr(C)]
pub struct FFITransaction {
    version: i32,
    data: *const u8,
    data_len: usize,
}

/// Block time for Golden River algorithm
#[repr(C)]
pub struct BlockTime {
    height: u32,
    timestamp: i64,
}

// ========== GOLDCOIN-SPECIFIC FFI FUNCTIONS ==========

/// Calculate Golden River difficulty adjustment
#[no_mangle]
pub extern "C" fn rust_golden_river(
    last_60_blocks_ptr: *const BlockTime,
    last_60_count: usize,
    last_120_blocks_ptr: *const BlockTime,
    last_120_count: usize,
    last_bits: u32,
    bits_60_ago: u32,
    bits_240_ago: u32,
    height: u32,
) -> u32 {
    if last_60_blocks_ptr.is_null() || last_120_blocks_ptr.is_null() {
        return last_bits; // Return current bits on error
    }
    
    let golden_river = GoldenRiverDifficulty::new();
    
    // Convert C arrays to Rust vectors
    let last_60_blocks = unsafe {
        std::slice::from_raw_parts(last_60_blocks_ptr, last_60_count)
            .iter()
            .map(|bt| (bt.height, bt.timestamp))
            .collect::<Vec<_>>()
    };
    
    let last_120_blocks = unsafe {
        std::slice::from_raw_parts(last_120_blocks_ptr, last_120_count)
            .iter()
            .map(|bt| (bt.height, bt.timestamp))
            .collect::<Vec<_>>()
    };
    
    // Calculate new difficulty
    golden_river.golden_river(
        &last_60_blocks,
        &last_120_blocks,
        last_bits,
        bits_60_ago,
        bits_240_ago,
        height,
    )
}

/// Check 51% defense
#[no_mangle]
pub extern "C" fn rust_check_51_defense(
    block_time: u32,
    prev_5th_block_time: u32,
    height: u32,
) -> bool {
    let defense = DefenseSystem::new();
    defense.check_51_percent_defense(block_time, prev_5th_block_time, height).is_ok()
}

/// Check timestamp limits for 51% defense
#[no_mangle]
pub extern "C" fn rust_check_timestamp_limits(
    block_time: i64,
    adjusted_time: i64,
    height: u32,
    is_local: bool,
) -> bool {
    let defense = DefenseSystem::new();
    defense.check_timestamp_limits(block_time, adjusted_time, height, is_local).is_ok()
}

/// Add automatic checkpoint
#[no_mangle]
pub extern "C" fn rust_add_auto_checkpoint(
    height: u32,
    ancestor_10_hash: *const u8,
) {
    if ancestor_10_hash.is_null() {
        return;
    }
    
    let hash = unsafe {
        let slice = std::slice::from_raw_parts(ancestor_10_hash, 32);
        let mut hash = [0u8; 32];
        hash.copy_from_slice(slice);
        hash
    };
    
    // In a real implementation, we'd store this in a persistent DefenseSystem
    // For now, just log it
    println!("Auto-checkpoint added at height {} (10th ancestor)", height - 10);
}

// ========== NETWORK FFI FUNCTIONS ==========

/// Create a new network manager
#[no_mangle]
pub extern "C" fn goldcoin_network_create(config: *const FFINetworkConfig) -> *mut NetworkHandle {
    if config.is_null() {
        return ptr::null_mut();
    }
    
    let config = unsafe { &*config };
    
    let user_agent = if config.user_agent.is_null() {
        String::from("/Goldcoin:0.17.0(Rust)/")
    } else {
        unsafe {
            CStr::from_ptr(config.user_agent)
                .to_string_lossy()
                .into_owned()
        }
    };
    
    let network_config = NetworkConfig {
        listen_addr: format!("0.0.0.0:{}", config.listen_port).parse().unwrap(),
        max_peers: config.max_peers,
        max_outbound: config.max_outbound,
        protocol_version: config.protocol_version,
        user_agent,
        relay: config.relay,
        services: config.services,
    };
    
    let runtime = tokio::runtime::Runtime::new().unwrap();
    let manager = Arc::new(NetworkManager::new(network_config));
    
    let handle = NetworkHandle {
        manager,
        runtime,
    };
    
    Box::into_raw(Box::new(handle))
}

/// Start the network manager
#[no_mangle]
pub extern "C" fn goldcoin_network_start(handle: *mut NetworkHandle) -> FFIResult {
    if handle.is_null() {
        return FFIResult::err("Invalid network handle".to_string());
    }
    
    let handle = unsafe { &mut *handle };
    
    let manager = handle.manager.clone();
    let result = handle.runtime.block_on(async move {
        manager.start().await
    });
    
    match result {
        Ok(()) => FFIResult::ok(),
        Err(e) => FFIResult::err(e.to_string()),
    }
}

/// Stop the network manager
#[no_mangle]
pub extern "C" fn goldcoin_network_stop(handle: *mut NetworkHandle) -> FFIResult {
    if handle.is_null() {
        return FFIResult::err("Invalid network handle".to_string());
    }
    
    let handle = unsafe { &mut *handle };
    
    let manager = handle.manager.clone();
    handle.runtime.block_on(async move {
        manager.shutdown().await
    });
    
    FFIResult::ok()
}

/// Free network manager handle
#[no_mangle]
pub extern "C" fn goldcoin_network_free(handle: *mut NetworkHandle) {
    if !handle.is_null() {
        unsafe {
            let _ = Box::from_raw(handle);
        }
    }
}

/// Get peer count
#[no_mangle]
pub extern "C" fn goldcoin_network_peer_count(handle: *const NetworkHandle) -> usize {
    if handle.is_null() {
        return 0;
    }
    
    let handle = unsafe { &*handle };
    handle.manager.peers.len()
}

// ========== CONSENSUS FFI FUNCTIONS ==========

/// Create a new chain state
#[no_mangle]
pub extern "C" fn goldcoin_chain_create() -> *mut ChainHandle {
    let runtime = tokio::runtime::Runtime::new().unwrap();
    let state = Arc::new(tokio::sync::RwLock::new(ChainState::new()));
    
    let handle = ChainHandle {
        state,
        runtime,
    };
    
    Box::into_raw(Box::new(handle))
}

/// Validate a block
#[no_mangle]
pub extern "C" fn goldcoin_validate_block(
    handle: *mut ChainHandle,
    header: *const FFIBlockHeader,
    txs: *const FFITransaction,
    tx_count: usize,
) -> FFIResult {
    if handle.is_null() || header.is_null() {
        return FFIResult::err("Invalid parameters".to_string());
    }
    
    let handle = unsafe { &mut *handle };
    let header = unsafe { &*header };
    
    // Convert FFI header to Rust type
    let block_header = goldcoin_consensus::BlockHeader {
        version: header.version,
        prev_block: header.prev_block,
        merkle_root: header.merkle_root,
        timestamp: header.timestamp,
        bits: header.bits,
        nonce: header.nonce,
    };
    
    // Convert transactions
    let mut transactions = Vec::new();
    if !txs.is_null() && tx_count > 0 {
        let tx_slice = unsafe { slice::from_raw_parts(txs, tx_count) };
        for ffi_tx in tx_slice {
            if !ffi_tx.data.is_null() && ffi_tx.data_len > 0 {
                let tx_data = unsafe { slice::from_raw_parts(ffi_tx.data, ffi_tx.data_len) };
                // TODO: Deserialize transaction from bytes
                // For now, create dummy transaction
                transactions.push(Transaction {
                    version: ffi_tx.version,
                    inputs: vec![],
                    outputs: vec![],
                    lock_time: 0,
                });
            }
        }
    }
    
    let block = Block {
        header: block_header,
        transactions,
    };
    
    // Validate block asynchronously
    let result = handle.runtime.block_on(async move {
        block.validate().await
    });
    
    match result {
        Ok(()) => FFIResult::ok(),
        Err(e) => FFIResult::err(e.to_string()),
    }
}

/// Connect a block to the chain
#[no_mangle]
pub extern "C" fn goldcoin_connect_block(
    handle: *mut ChainHandle,
    header: *const FFIBlockHeader,
    txs: *const FFITransaction,
    tx_count: usize,
    height: u32,
) -> FFIResult {
    if handle.is_null() || header.is_null() {
        return FFIResult::err("Invalid parameters".to_string());
    }
    
    let handle = unsafe { &mut *handle };
    let header = unsafe { &*header };
    
    // Convert FFI types to Rust types
    let block_header = goldcoin_consensus::BlockHeader {
        version: header.version,
        prev_block: header.prev_block,
        merkle_root: header.merkle_root,
        timestamp: header.timestamp,
        bits: header.bits,
        nonce: header.nonce,
    };
    
    let mut transactions = Vec::new();
    if !txs.is_null() && tx_count > 0 {
        let tx_slice = unsafe { slice::from_raw_parts(txs, tx_count) };
        for ffi_tx in tx_slice {
            if !ffi_tx.data.is_null() && ffi_tx.data_len > 0 {
                // TODO: Deserialize transaction
                transactions.push(Transaction {
                    version: ffi_tx.version,
                    inputs: vec![],
                    outputs: vec![],
                    lock_time: 0,
                });
            }
        }
    }
    
    let block = Block {
        header: block_header,
        transactions,
    };
    
    // Connect block asynchronously
    let state = handle.state.clone();
    let result = handle.runtime.block_on(async move {
        let mut state = state.write().await;
        state.connect_block(&block, height).await
    });
    
    match result {
        Ok(()) => FFIResult::ok(),
        Err(e) => FFIResult::err(e.to_string()),
    }
}

/// Get best block height
#[no_mangle]
pub extern "C" fn goldcoin_get_best_height(handle: *const ChainHandle) -> u32 {
    if handle.is_null() {
        return 0;
    }
    
    let handle = unsafe { &*handle };
    
    let state = handle.state.clone();
    handle.runtime.block_on(async move {
        let state = state.read().await;
        state.best_height
    })
}

/// Get best block hash
#[no_mangle]
pub extern "C" fn goldcoin_get_best_hash(handle: *const ChainHandle, out_hash: *mut u8) {
    if handle.is_null() || out_hash.is_null() {
        return;
    }
    
    let handle = unsafe { &*handle };
    
    let state = handle.state.clone();
    let hash = handle.runtime.block_on(async move {
        let state = state.read().await;
        state.best_block
    });
    
    unsafe {
        ptr::copy_nonoverlapping(hash.as_ptr(), out_hash, 32);
    }
}

/// Free chain state handle
#[no_mangle]
pub extern "C" fn goldcoin_chain_free(handle: *mut ChainHandle) {
    if !handle.is_null() {
        unsafe {
            let _ = Box::from_raw(handle);
        }
    }
}

/// Free FFI result error message
#[no_mangle]
pub extern "C" fn goldcoin_result_free_error(result: FFIResult) {
    if !result.error_msg.is_null() {
        unsafe {
            let _ = CString::from_raw(result.error_msg as *mut c_char);
        }
    }
}

// ========== PERFORMANCE MONITORING ==========

/// Get network statistics
#[repr(C)]
pub struct NetworkStats {
    peer_count: usize,
    bytes_sent: u64,
    bytes_recv: u64,
    messages_sent: u64,
    messages_recv: u64,
}

#[no_mangle]
pub extern "C" fn goldcoin_network_get_stats(handle: *const NetworkHandle) -> NetworkStats {
    if handle.is_null() {
        return NetworkStats {
            peer_count: 0,
            bytes_sent: 0,
            bytes_recv: 0,
            messages_sent: 0,
            messages_recv: 0,
        };
    }
    
    let handle = unsafe { &*handle };
    
    // TODO: Collect actual stats from network manager
    NetworkStats {
        peer_count: handle.manager.peers.len(),
        bytes_sent: 0,
        bytes_recv: 0,
        messages_sent: 0,
        messages_recv: 0,
    }
}

/// Get chain statistics
#[repr(C)]
pub struct ChainStats {
    best_height: u32,
    utxo_count: usize,
    total_work: [u8; 32],
}

#[no_mangle]
pub extern "C" fn goldcoin_chain_get_stats(handle: *const ChainHandle) -> ChainStats {
    if handle.is_null() {
        return ChainStats {
            best_height: 0,
            utxo_count: 0,
            total_work: [0u8; 32],
        };
    }
    
    let handle = unsafe { &*handle };
    
    let state = handle.state.clone();
    handle.runtime.block_on(async move {
        let state = state.read().await;
        ChainStats {
            best_height: state.best_height,
            utxo_count: state.utxos.len(),
            total_work: state.chain_work,
        }
    })
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_network_create_and_free() {
        let config = FFINetworkConfig {
            listen_port: 51241,
            max_peers: 125,
            max_outbound: 8,
            protocol_version: 70018,
            user_agent: ptr::null(),
            relay: true,
            services: 1,
        };
        
        let handle = goldcoin_network_create(&config);
        assert!(!handle.is_null());
        
        goldcoin_network_free(handle);
    }
    
    #[test]
    fn test_chain_create_and_free() {
        let handle = goldcoin_chain_create();
        assert!(!handle.is_null());
        
        let height = goldcoin_get_best_height(handle);
        assert_eq!(height, 0);
        
        goldcoin_chain_free(handle);
    }
}