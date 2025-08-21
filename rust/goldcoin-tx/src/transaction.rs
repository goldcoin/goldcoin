// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! Transaction structures and serialization

use anyhow::{Context, Result};
use serde_json::{Value, json};
use sha2::{Sha256, Digest};

use crate::script::Script;

/// A Goldcoin transaction input
#[derive(Debug, Clone)]
pub struct TxInput {
    /// Previous transaction output (txid, vout)
    pub previous_output: (Vec<u8>, u32),
    /// Script signature
    pub script_sig: Script,
    /// Sequence number
    pub sequence: u32,
}

/// A Goldcoin transaction output
#[derive(Debug, Clone)]
pub struct TxOutput {
    /// Value in satoshis
    pub value: u64,
    /// Script public key
    pub script_pubkey: Script,
}

/// A complete Goldcoin transaction
#[derive(Debug, Clone)]
pub struct Transaction {
    /// Transaction version
    version: u32,
    /// Transaction inputs
    inputs: Vec<TxInput>,
    /// Transaction outputs
    outputs: Vec<TxOutput>,
    /// Lock time
    locktime: u32,
}

impl Transaction {
    /// Create a new empty transaction
    pub fn new() -> Self {
        Self {
            version: 2, // BIP 68 support
            inputs: Vec::new(),
            outputs: Vec::new(),
            locktime: 0,
        }
    }
    
    /// Parse transaction from hex string
    pub fn from_hex(hex: &str) -> Result<Self> {
        let bytes = hex::decode(hex.trim())
            .context("Invalid hex string")?;
        Self::from_bytes(&bytes)
    }
    
    /// Parse transaction from bytes
    pub fn from_bytes(bytes: &[u8]) -> Result<Self> {
        let mut cursor = 0;
        
        // Version (4 bytes, little endian)
        if bytes.len() < 4 {
            return Err(anyhow::anyhow!("Transaction too short"));
        }
        let version = u32::from_le_bytes([bytes[0], bytes[1], bytes[2], bytes[3]]);
        cursor += 4;
        
        // Input count (varint)
        let (input_count, varint_size) = read_varint(&bytes[cursor..])?;
        cursor += varint_size;
        
        // Parse inputs
        let mut inputs = Vec::new();
        for _ in 0..input_count {
            let (input, input_size) = parse_input(&bytes[cursor..])?;
            inputs.push(input);
            cursor += input_size;
        }
        
        // Output count (varint)
        let (output_count, varint_size) = read_varint(&bytes[cursor..])?;
        cursor += varint_size;
        
        // Parse outputs
        let mut outputs = Vec::new();
        for _ in 0..output_count {
            let (output, output_size) = parse_output(&bytes[cursor..])?;
            outputs.push(output);
            cursor += output_size;
        }
        
        // Locktime (4 bytes, little endian)
        if bytes.len() < cursor + 4 {
            return Err(anyhow::anyhow!("Transaction truncated"));
        }
        let locktime = u32::from_le_bytes([
            bytes[cursor], bytes[cursor+1], bytes[cursor+2], bytes[cursor+3]
        ]);
        
        Ok(Self {
            version,
            inputs,
            outputs,
            locktime,
        })
    }
    
    /// Parse transaction from JSON
    pub fn from_json(json: &Value) -> Result<Self> {
        let version = json.get("version")
            .and_then(|v| v.as_u64())
            .unwrap_or(2) as u32;
        
        let locktime = json.get("locktime")
            .and_then(|v| v.as_u64())
            .unwrap_or(0) as u32;
        
        let mut inputs = Vec::new();
        if let Some(vin) = json.get("vin").and_then(|v| v.as_array()) {
            for input_json in vin {
                let txid_hex = input_json.get("txid")
                    .and_then(|v| v.as_str())
                    .context("Missing txid in input")?;
                let vout = input_json.get("vout")
                    .and_then(|v| v.as_u64())
                    .context("Missing vout in input")? as u32;
                let sequence = input_json.get("sequence")
                    .and_then(|v| v.as_u64())
                    .unwrap_or(0xfffffffe) as u32;
                
                let txid = hex::decode(txid_hex)
                    .context("Invalid txid hex")?;
                
                inputs.push(TxInput {
                    previous_output: (txid, vout),
                    script_sig: Script::new(),
                    sequence,
                });
            }
        }
        
        let mut outputs = Vec::new();
        if let Some(vout) = json.get("vout").and_then(|v| v.as_array()) {
            for output_json in vout {
                let value = output_json.get("value")
                    .and_then(|v| v.as_f64())
                    .context("Missing value in output")?;
                let value_satoshis = (value * 100_000_000.0) as u64;
                
                outputs.push(TxOutput {
                    value: value_satoshis,
                    script_pubkey: Script::new(),
                });
            }
        }
        
        Ok(Self {
            version,
            inputs,
            outputs,
            locktime,
        })
    }
    
    /// Serialize transaction to hex string
    pub fn to_hex(&self) -> String {
        hex::encode(self.to_bytes())
    }
    
    /// Serialize transaction to bytes
    pub fn to_bytes(&self) -> Vec<u8> {
        let mut bytes = Vec::new();
        
        // Version
        bytes.extend_from_slice(&self.version.to_le_bytes());
        
        // Input count
        bytes.extend_from_slice(&encode_varint(self.inputs.len() as u64));
        
        // Inputs
        for input in &self.inputs {
            bytes.extend_from_slice(&serialize_input(input));
        }
        
        // Output count
        bytes.extend_from_slice(&encode_varint(self.outputs.len() as u64));
        
        // Outputs
        for output in &self.outputs {
            bytes.extend_from_slice(&serialize_output(output));
        }
        
        // Locktime
        bytes.extend_from_slice(&self.locktime.to_le_bytes());
        
        bytes
    }
    
    /// Convert transaction to JSON
    pub fn to_json(&self) -> Value {
        let mut vin = Vec::new();
        for input in &self.inputs {
            vin.push(json!({
                "txid": hex::encode(&input.previous_output.0),
                "vout": input.previous_output.1,
                "sequence": input.sequence
            }));
        }
        
        let mut vout = Vec::new();
        for (n, output) in self.outputs.iter().enumerate() {
            vout.push(json!({
                "value": output.value as f64 / 100_000_000.0,
                "n": n,
                "scriptPubKey": {
                    "hex": output.script_pubkey.to_hex()
                }
            }));
        }
        
        json!({
            "txid": hex::encode(self.txid()),
            "version": self.version,
            "locktime": self.locktime,
            "vin": vin,
            "vout": vout
        })
    }
    
    /// Calculate transaction ID (double SHA256)
    pub fn txid(&self) -> Vec<u8> {
        let bytes = self.to_bytes();
        let hash1 = Sha256::digest(&bytes);
        let hash2 = Sha256::digest(&hash1);
        hash2.to_vec()
    }
    
    /// Add an input to the transaction
    pub fn add_input(&mut self, input: TxInput) {
        self.inputs.push(input);
    }
    
    /// Add an output to the transaction
    pub fn add_output(&mut self, output: TxOutput) {
        self.outputs.push(output);
    }
    
    /// Remove an input at the specified index
    pub fn remove_input(&mut self, index: usize) -> Result<()> {
        if index >= self.inputs.len() {
            return Err(anyhow::anyhow!("Input index {} out of range", index));
        }
        self.inputs.remove(index);
        Ok(())
    }
    
    /// Remove an output at the specified index
    pub fn remove_output(&mut self, index: usize) -> Result<()> {
        if index >= self.outputs.len() {
            return Err(anyhow::anyhow!("Output index {} out of range", index));
        }
        self.outputs.remove(index);
        Ok(())
    }
    
    /// Set transaction locktime
    pub fn set_locktime(&mut self, locktime: u32) {
        self.locktime = locktime;
    }
    
    /// Set RBF (Replace-By-Fee) flag on all inputs
    pub fn set_rbf_flag(&mut self) {
        for input in &mut self.inputs {
            if input.sequence == 0xffffffff {
                input.sequence = 0xfffffffe; // Enable RBF
            }
        }
    }
    
    /// Get inputs
    pub fn inputs(&self) -> &[TxInput] {
        &self.inputs
    }
    
    /// Get outputs
    pub fn outputs(&self) -> &[TxOutput] {
        &self.outputs
    }
    
    /// Get locktime
    pub fn locktime(&self) -> u32 {
        self.locktime
    }
}

// Helper functions for serialization

fn read_varint(bytes: &[u8]) -> Result<(u64, usize)> {
    if bytes.is_empty() {
        return Err(anyhow::anyhow!("Empty varint"));
    }
    
    match bytes[0] {
        0..=0xfc => Ok((bytes[0] as u64, 1)),
        0xfd => {
            if bytes.len() < 3 {
                return Err(anyhow::anyhow!("Truncated varint"));
            }
            let value = u16::from_le_bytes([bytes[1], bytes[2]]) as u64;
            Ok((value, 3))
        }
        0xfe => {
            if bytes.len() < 5 {
                return Err(anyhow::anyhow!("Truncated varint"));
            }
            let value = u32::from_le_bytes([bytes[1], bytes[2], bytes[3], bytes[4]]) as u64;
            Ok((value, 5))
        }
        0xff => {
            if bytes.len() < 9 {
                return Err(anyhow::anyhow!("Truncated varint"));
            }
            let value = u64::from_le_bytes([
                bytes[1], bytes[2], bytes[3], bytes[4],
                bytes[5], bytes[6], bytes[7], bytes[8]
            ]);
            Ok((value, 9))
        }
    }
}

fn encode_varint(value: u64) -> Vec<u8> {
    if value < 0xfd {
        vec![value as u8]
    } else if value <= 0xffff {
        let mut bytes = vec![0xfd];
        bytes.extend_from_slice(&(value as u16).to_le_bytes());
        bytes
    } else if value <= 0xffffffff {
        let mut bytes = vec![0xfe];
        bytes.extend_from_slice(&(value as u32).to_le_bytes());
        bytes
    } else {
        let mut bytes = vec![0xff];
        bytes.extend_from_slice(&value.to_le_bytes());
        bytes
    }
}

fn parse_input(bytes: &[u8]) -> Result<(TxInput, usize)> {
    let mut cursor = 0;
    
    // Previous output hash (32 bytes)
    if bytes.len() < 32 {
        return Err(anyhow::anyhow!("Input too short"));
    }
    let prev_hash = bytes[cursor..cursor+32].to_vec();
    cursor += 32;
    
    // Previous output index (4 bytes)
    if bytes.len() < cursor + 4 {
        return Err(anyhow::anyhow!("Input too short"));
    }
    let prev_index = u32::from_le_bytes([
        bytes[cursor], bytes[cursor+1], bytes[cursor+2], bytes[cursor+3]
    ]);
    cursor += 4;
    
    // Script length (varint)
    let (script_len, varint_size) = read_varint(&bytes[cursor..])?;
    cursor += varint_size;
    
    // Script
    if bytes.len() < cursor + script_len as usize {
        return Err(anyhow::anyhow!("Input script too short"));
    }
    let script_bytes = bytes[cursor..cursor + script_len as usize].to_vec();
    cursor += script_len as usize;
    
    // Sequence (4 bytes)
    if bytes.len() < cursor + 4 {
        return Err(anyhow::anyhow!("Input too short"));
    }
    let sequence = u32::from_le_bytes([
        bytes[cursor], bytes[cursor+1], bytes[cursor+2], bytes[cursor+3]
    ]);
    cursor += 4;
    
    let input = TxInput {
        previous_output: (prev_hash, prev_index),
        script_sig: Script::from_bytes(script_bytes),
        sequence,
    };
    
    Ok((input, cursor))
}

fn parse_output(bytes: &[u8]) -> Result<(TxOutput, usize)> {
    let mut cursor = 0;
    
    // Value (8 bytes)
    if bytes.len() < 8 {
        return Err(anyhow::anyhow!("Output too short"));
    }
    let value = u64::from_le_bytes([
        bytes[0], bytes[1], bytes[2], bytes[3],
        bytes[4], bytes[5], bytes[6], bytes[7]
    ]);
    cursor += 8;
    
    // Script length (varint)
    let (script_len, varint_size) = read_varint(&bytes[cursor..])?;
    cursor += varint_size;
    
    // Script
    if bytes.len() < cursor + script_len as usize {
        return Err(anyhow::anyhow!("Output script too short"));
    }
    let script_bytes = bytes[cursor..cursor + script_len as usize].to_vec();
    cursor += script_len as usize;
    
    let output = TxOutput {
        value,
        script_pubkey: Script::from_bytes(script_bytes),
    };
    
    Ok((output, cursor))
}

fn serialize_input(input: &TxInput) -> Vec<u8> {
    let mut bytes = Vec::new();
    
    // Previous output hash
    bytes.extend_from_slice(&input.previous_output.0);
    
    // Previous output index
    bytes.extend_from_slice(&input.previous_output.1.to_le_bytes());
    
    // Script length and script
    let script_bytes = input.script_sig.as_bytes();
    bytes.extend_from_slice(&encode_varint(script_bytes.len() as u64));
    bytes.extend_from_slice(script_bytes);
    
    // Sequence
    bytes.extend_from_slice(&input.sequence.to_le_bytes());
    
    bytes
}

fn serialize_output(output: &TxOutput) -> Vec<u8> {
    let mut bytes = Vec::new();
    
    // Value
    bytes.extend_from_slice(&output.value.to_le_bytes());
    
    // Script length and script
    let script_bytes = output.script_pubkey.as_bytes();
    bytes.extend_from_slice(&encode_varint(script_bytes.len() as u64));
    bytes.extend_from_slice(script_bytes);
    
    bytes
}