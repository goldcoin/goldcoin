// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! Script handling for transactions

use anyhow::{Context, Result};
use sha2::{Sha256, Digest};
use ripemd::{Ripemd160, Digest as RipemdDigest};

/// Bitcoin/Goldcoin script opcodes
#[allow(dead_code)]
mod opcodes {
    pub const OP_DUP: u8 = 0x76;
    pub const OP_HASH160: u8 = 0xa9;
    pub const OP_EQUALVERIFY: u8 = 0x88;
    pub const OP_CHECKSIG: u8 = 0xac;
    pub const OP_EQUAL: u8 = 0x87;
}

/// A Bitcoin/Goldcoin script
#[derive(Debug, Clone)]
pub struct Script {
    bytes: Vec<u8>,
}

impl Script {
    /// Create a new empty script
    pub fn new() -> Self {
        Self { bytes: Vec::new() }
    }
    
    /// Create script from bytes
    pub fn from_bytes(bytes: Vec<u8>) -> Self {
        Self { bytes }
    }
    
    /// Create script from hex string
    pub fn from_hex(hex: &str) -> Result<Self> {
        let bytes = hex::decode(hex.trim())
            .context("Invalid hex string")?;
        Ok(Self { bytes })
    }
    
    /// Create a P2PKH (Pay to Public Key Hash) script from address
    pub fn from_address(address: &str) -> Result<Self> {
        // This is a simplified implementation
        // In a real implementation, you would parse the address properly
        // and handle different address formats (P2PKH, P2SH, Bech32, etc.)
        
        if address.starts_with("G") || address.starts_with("1") {
            // Assume P2PKH address
            let pubkey_hash = decode_base58_address(address)?;
            Ok(Self::p2pkh_script(&pubkey_hash))
        } else if address.starts_with("3") {
            // Assume P2SH address
            let script_hash = decode_base58_address(address)?;
            Ok(Self::p2sh_script(&script_hash))
        } else {
            Err(anyhow::anyhow!("Unsupported address format: {}", address))
        }
    }
    
    /// Create a P2PKH script
    fn p2pkh_script(pubkey_hash: &[u8]) -> Self {
        let mut script = Vec::new();
        script.push(opcodes::OP_DUP);
        script.push(opcodes::OP_HASH160);
        script.push(20); // Push 20 bytes
        script.extend_from_slice(pubkey_hash);
        script.push(opcodes::OP_EQUALVERIFY);
        script.push(opcodes::OP_CHECKSIG);
        
        Self { bytes: script }
    }
    
    /// Create a P2SH script
    fn p2sh_script(script_hash: &[u8]) -> Self {
        let mut script = Vec::new();
        script.push(opcodes::OP_HASH160);
        script.push(20); // Push 20 bytes
        script.extend_from_slice(script_hash);
        script.push(opcodes::OP_EQUAL);
        
        Self { bytes: script }
    }
    
    /// Get script as bytes
    pub fn as_bytes(&self) -> &[u8] {
        &self.bytes
    }
    
    /// Convert script to hex string
    pub fn to_hex(&self) -> String {
        hex::encode(&self.bytes)
    }
    
    /// Check if script is empty
    pub fn is_empty(&self) -> bool {
        self.bytes.is_empty()
    }
    
    /// Get script length
    pub fn len(&self) -> usize {
        self.bytes.len()
    }
}

/// Simplified Base58 address decoding
/// In a real implementation, this would include proper checksum validation
fn decode_base58_address(address: &str) -> Result<Vec<u8>> {
    // This is a placeholder implementation
    // A real implementation would use a proper Base58 decoder
    // and validate the checksum
    
    // For now, return a dummy 20-byte hash
    // In practice, this would decode the address and extract the hash160
    let mut hasher = Ripemd160::new();
    hasher.update(address.as_bytes());
    let result = hasher.finalize();
    Ok(result.to_vec())
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_empty_script() {
        let script = Script::new();
        assert!(script.is_empty());
        assert_eq!(script.len(), 0);
    }
    
    #[test]
    fn test_script_from_hex() {
        let hex = "76a914deadbeefdeadbeefdeadbeefdeadbeefdeadbeef88ac";
        let script = Script::from_hex(hex).unwrap();
        assert_eq!(script.to_hex(), hex);
    }
    
    #[test]
    fn test_p2pkh_script_creation() {
        let pubkey_hash = vec![0; 20]; // Dummy hash
        let script = Script::p2pkh_script(&pubkey_hash);
        assert!(!script.is_empty());
        assert_eq!(script.len(), 25); // Standard P2PKH script length
    }
}