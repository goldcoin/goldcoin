// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! Transaction signing utilities

use anyhow::{Context, Result};
use secp256k1::{Secp256k1, SecretKey, PublicKey, Message};
use sha2::{Sha256, Digest};

use crate::transaction::Transaction;
use crate::script::Script;

/// Sign a transaction using private keys from a file
pub fn sign_transaction(tx: &mut Transaction, keyfile: &str) -> Result<()> {
    let keys = load_private_keys(keyfile)?;
    
    // For each input, try to find a matching private key and sign
    for (i, input) in tx.inputs().iter().enumerate() {
        if let Some(signature) = sign_input(tx, i, &keys)? {
            // Update the input's script signature
            // This is a simplified implementation
            // In practice, you would properly construct the scriptSig
            println!("Signed input {} with signature: {}", i, hex::encode(&signature));
        }
    }
    
    Ok(())
}

/// Load private keys from a file
fn load_private_keys(filename: &str) -> Result<Vec<SecretKey>> {
    let content = std::fs::read_to_string(filename)
        .context(format!("Failed to read key file: {}", filename))?;
    
    let mut keys = Vec::new();
    for line in content.lines() {
        let line = line.trim();
        if line.is_empty() || line.starts_with('#') {
            continue;
        }
        
        // Try to parse as hex private key
        if let Ok(key_bytes) = hex::decode(line) {
            if key_bytes.len() == 32 {
                let secp = Secp256k1::new();
                if let Ok(secret_key) = SecretKey::from_slice(&key_bytes) {
                    keys.push(secret_key);
                }
            }
        }
    }
    
    if keys.is_empty() {
        return Err(anyhow::anyhow!("No valid private keys found in file"));
    }
    
    Ok(keys)
}

/// Sign a specific input of a transaction
fn sign_input(tx: &Transaction, input_index: usize, keys: &[SecretKey]) -> Result<Option<Vec<u8>>> {
    let secp = Secp256k1::new();
    
    // Create the signature hash for this input
    let sighash = calculate_signature_hash(tx, input_index)?;
    let message = Message::from_digest_slice(&sighash)
        .context("Failed to create message from sighash")?;
    
    // Try each private key to see if it can sign this input
    for secret_key in keys {
        let public_key = PublicKey::from_secret_key(&secp, secret_key);
        
        // Check if this public key corresponds to the input's script
        // This is a simplified check - in practice you would analyze the scriptPubKey
        // of the previous output to determine the required signature
        
        // For now, just create a signature
        let signature = secp.sign_ecdsa(&message, secret_key);
        let signature_bytes = signature.serialize_der();
        
        return Ok(Some(signature_bytes.to_vec()));
    }
    
    Ok(None)
}

/// Calculate the signature hash for a transaction input
fn calculate_signature_hash(tx: &Transaction, input_index: usize) -> Result<[u8; 32]> {
    // This is a simplified signature hash calculation
    // In practice, this would follow the exact Bitcoin signature hash algorithm
    // including handling of SIGHASH flags, witness data, etc.
    
    if input_index >= tx.inputs().len() {
        return Err(anyhow::anyhow!("Input index out of range"));
    }
    
    // For this simplified implementation, just hash the transaction bytes
    // with the input index appended
    let mut hasher = Sha256::new();
    hasher.update(tx.to_bytes());
    hasher.update(&(input_index as u32).to_le_bytes());
    hasher.update(&[0x01, 0x00, 0x00, 0x00]); // SIGHASH_ALL
    
    let hash1 = hasher.finalize();
    let mut hasher2 = Sha256::new();
    hasher2.update(hash1);
    let hash2 = hasher2.finalize();
    
    let mut result = [0u8; 32];
    result.copy_from_slice(&hash2);
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::transaction::{Transaction, TxInput, TxOutput};
    use std::io::Write;
    use tempfile::NamedTempFile;
    
    #[test]
    fn test_private_key_loading() {
        let mut temp_file = NamedTempFile::new().unwrap();
        writeln!(temp_file, "# Test private key file").unwrap();
        writeln!(temp_file, "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef").unwrap();
        writeln!(temp_file, "").unwrap();
        writeln!(temp_file, "fedcba9876543210fedcba9876543210fedcba9876543210fedcba9876543210").unwrap();
        
        let keys = load_private_keys(temp_file.path().to_str().unwrap()).unwrap();
        assert_eq!(keys.len(), 2);
    }
    
    #[test]
    fn test_signature_hash_calculation() {
        let tx = Transaction::new();
        
        // Since the transaction is empty, this should still work but return a hash
        // In practice, you wouldn't sign an empty transaction
        let result = calculate_signature_hash(&tx, 0);
        assert!(result.is_err()); // Should fail for empty transaction
    }
}