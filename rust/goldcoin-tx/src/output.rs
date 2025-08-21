// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! Transaction output utilities

use anyhow::{Context, Result};
use crate::script::Script;

/// Parse output specification from command line
pub fn parse_output_spec(spec: &str) -> Result<(String, u64)> {
    let parts: Vec<&str> = spec.split(':').collect();
    
    if parts.len() != 2 {
        return Err(anyhow::anyhow!("Invalid output format: {}", spec));
    }
    
    let address = parts[0].to_string();
    let amount: f64 = parts[1].parse()
        .context("Invalid amount")?;
    
    if amount < 0.0 {
        return Err(anyhow::anyhow!("Amount cannot be negative"));
    }
    
    // Convert to satoshis
    let amount_satoshis = (amount * 100_000_000.0) as u64;
    
    Ok((address, amount_satoshis))
}

/// Validate Goldcoin address format
pub fn validate_address(address: &str) -> Result<()> {
    if address.is_empty() {
        return Err(anyhow::anyhow!("Empty address"));
    }
    
    // Basic validation - in practice this would be more comprehensive
    if !address.starts_with('G') && !address.starts_with('1') && !address.starts_with('3') {
        return Err(anyhow::anyhow!("Invalid address format"));
    }
    
    if address.len() < 26 || address.len() > 35 {
        return Err(anyhow::anyhow!("Invalid address length"));
    }
    
    Ok(())
}