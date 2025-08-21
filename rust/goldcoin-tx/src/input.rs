// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! Transaction input utilities

use anyhow::{Context, Result};
use crate::script::Script;

/// Parse input specification from command line
pub fn parse_input_spec(spec: &str) -> Result<(Vec<u8>, u32, u32)> {
    let parts: Vec<&str> = spec.split(':').collect();
    
    if parts.len() < 2 {
        return Err(anyhow::anyhow!("Invalid input format: {}", spec));
    }
    
    let txid = hex::decode(parts[0])
        .context("Invalid transaction ID hex")?;
    
    if txid.len() != 32 {
        return Err(anyhow::anyhow!("Transaction ID must be 32 bytes"));
    }
    
    let vout: u32 = parts[1].parse()
        .context("Invalid output index")?;
    
    let sequence = if parts.len() > 2 {
        parts[2].parse().unwrap_or(0xfffffffe)
    } else {
        0xfffffffe // Default for RBF
    };
    
    Ok((txid, vout, sequence))
}