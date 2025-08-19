//! Bitcoin Script implementation for Goldcoin
//! 
//! This module provides script evaluation and validation for transactions

use anyhow::Result;
use std::fmt;

/// Represents a Bitcoin Script
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Script {
    bytes: Vec<u8>,
}

impl Script {
    /// Create a new empty script
    pub fn new() -> Self {
        Script { bytes: Vec::new() }
    }

    /// Create script from bytes
    pub fn from_bytes(bytes: Vec<u8>) -> Self {
        Script { bytes }
    }

    /// Get script as bytes
    pub fn as_bytes(&self) -> &[u8] {
        &self.bytes
    }

    /// Check if script is empty
    pub fn is_empty(&self) -> bool {
        self.bytes.is_empty()
    }

    /// Get script length
    pub fn len(&self) -> usize {
        self.bytes.len()
    }

    /// Verify script execution
    pub fn verify(&self, _stack: &[Vec<u8>]) -> Result<bool> {
        // TODO: Implement script verification logic
        Ok(true)
    }

    /// Check if script is P2PKH (Pay to Public Key Hash)
    pub fn is_p2pkh(&self) -> bool {
        self.bytes.len() == 25
            && self.bytes[0] == 0x76  // OP_DUP
            && self.bytes[1] == 0xa9  // OP_HASH160
            && self.bytes[2] == 0x14  // Push 20 bytes
            && self.bytes[23] == 0x88 // OP_EQUALVERIFY
            && self.bytes[24] == 0xac // OP_CHECKSIG
    }

    /// Check if script is P2SH (Pay to Script Hash)
    pub fn is_p2sh(&self) -> bool {
        self.bytes.len() == 23
            && self.bytes[0] == 0xa9  // OP_HASH160
            && self.bytes[1] == 0x14  // Push 20 bytes
            && self.bytes[22] == 0x87 // OP_EQUAL
    }
}

impl Default for Script {
    fn default() -> Self {
        Self::new()
    }
}

impl fmt::Display for Script {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "Script({})", hex::encode(&self.bytes))
    }
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
    fn test_p2pkh_detection() {
        let p2pkh_bytes = vec![
            0x76, 0xa9, 0x14, // OP_DUP OP_HASH160 PUSH(20)
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x88, 0xac, // OP_EQUALVERIFY OP_CHECKSIG
        ];
        let script = Script::from_bytes(p2pkh_bytes);
        assert!(script.is_p2pkh());
        assert!(!script.is_p2sh());
    }
}