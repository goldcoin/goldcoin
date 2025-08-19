// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! Bitcoin-compatible serialization for Goldcoin transactions
//! Zero-copy where possible, lightning fast!

use crate::{Transaction, TxInput, TxOutput};
use std::io::{self, Read, Write};

pub trait Serialize {
    fn serialize<W: Write>(&self, writer: &mut W) -> io::Result<()>;
}

pub trait Deserialize: Sized {
    fn deserialize<R: Read>(reader: &mut R) -> io::Result<Self>;
}

// Compact size encoding (Bitcoin's VarInt)
pub fn write_compact_size<W: Write>(writer: &mut W, size: u64) -> io::Result<()> {
    match size {
        0..=0xFC => writer.write_all(&[size as u8])?,
        0xFD..=0xFFFF => {
            writer.write_all(&[0xFD])?;
            writer.write_all(&(size as u16).to_le_bytes())?;
        }
        0x10000..=0xFFFFFFFF => {
            writer.write_all(&[0xFE])?;
            writer.write_all(&(size as u32).to_le_bytes())?;
        }
        _ => {
            writer.write_all(&[0xFF])?;
            writer.write_all(&size.to_le_bytes())?;
        }
    }
    Ok(())
}

pub fn read_compact_size<R: Read>(reader: &mut R) -> io::Result<u64> {
    let mut marker = [0u8; 1];
    reader.read_exact(&mut marker)?;
    
    match marker[0] {
        0..=0xFC => Ok(marker[0] as u64),
        0xFD => {
            let mut buf = [0u8; 2];
            reader.read_exact(&mut buf)?;
            Ok(u16::from_le_bytes(buf) as u64)
        }
        0xFE => {
            let mut buf = [0u8; 4];
            reader.read_exact(&mut buf)?;
            Ok(u32::from_le_bytes(buf) as u64)
        }
        0xFF => {
            let mut buf = [0u8; 8];
            reader.read_exact(&mut buf)?;
            Ok(u64::from_le_bytes(buf))
        }
    }
}

impl Serialize for Transaction {
    fn serialize<W: Write>(&self, writer: &mut W) -> io::Result<()> {
        // Version
        writer.write_all(&self.version.to_le_bytes())?;
        
        // Input count
        write_compact_size(writer, self.inputs.len() as u64)?;
        
        // Inputs
        for input in &self.inputs {
            input.serialize(writer)?;
        }
        
        // Output count
        write_compact_size(writer, self.outputs.len() as u64)?;
        
        // Outputs
        for output in &self.outputs {
            output.serialize(writer)?;
        }
        
        // Lock time
        writer.write_all(&self.lock_time.to_le_bytes())?;
        
        Ok(())
    }
}

impl Deserialize for Transaction {
    fn deserialize<R: Read>(reader: &mut R) -> io::Result<Self> {
        // Version
        let mut version_bytes = [0u8; 4];
        reader.read_exact(&mut version_bytes)?;
        let version = u32::from_le_bytes(version_bytes);
        
        // Input count
        let input_count = read_compact_size(reader)?;
        
        // Inputs
        let mut inputs = Vec::with_capacity(input_count as usize);
        for _ in 0..input_count {
            inputs.push(TxInput::deserialize(reader)?);
        }
        
        // Output count
        let output_count = read_compact_size(reader)?;
        
        // Outputs
        let mut outputs = Vec::with_capacity(output_count as usize);
        for _ in 0..output_count {
            outputs.push(TxOutput::deserialize(reader)?);
        }
        
        // Lock time
        let mut lock_time_bytes = [0u8; 4];
        reader.read_exact(&mut lock_time_bytes)?;
        let lock_time = u32::from_le_bytes(lock_time_bytes);
        
        Ok(Transaction {
            version,
            inputs,
            outputs,
            lock_time,
        })
    }
}

impl Serialize for TxInput {
    fn serialize<W: Write>(&self, writer: &mut W) -> io::Result<()> {
        // Previous transaction hash
        writer.write_all(&self.prev_tx_hash)?;
        
        // Previous output index
        writer.write_all(&self.prev_index.to_le_bytes())?;
        
        // Script length
        write_compact_size(writer, self.script_sig.len() as u64)?;
        
        // Script
        writer.write_all(&self.script_sig)?;
        
        // Sequence
        writer.write_all(&self.sequence.to_le_bytes())?;
        
        Ok(())
    }
}

impl Deserialize for TxInput {
    fn deserialize<R: Read>(reader: &mut R) -> io::Result<Self> {
        // Previous transaction hash
        let mut prev_tx_hash = [0u8; 32];
        reader.read_exact(&mut prev_tx_hash)?;
        
        // Previous output index
        let mut prev_index_bytes = [0u8; 4];
        reader.read_exact(&mut prev_index_bytes)?;
        let prev_index = u32::from_le_bytes(prev_index_bytes);
        
        // Script length
        let script_len = read_compact_size(reader)?;
        
        // Script
        let mut script_sig = vec![0u8; script_len as usize];
        reader.read_exact(&mut script_sig)?;
        
        // Sequence
        let mut sequence_bytes = [0u8; 4];
        reader.read_exact(&mut sequence_bytes)?;
        let sequence = u32::from_le_bytes(sequence_bytes);
        
        Ok(TxInput {
            prev_tx_hash,
            prev_index,
            script_sig,
            sequence,
        })
    }
}

impl Serialize for TxOutput {
    fn serialize<W: Write>(&self, writer: &mut W) -> io::Result<()> {
        // Value
        writer.write_all(&self.value.to_le_bytes())?;
        
        // Script length
        write_compact_size(writer, self.script_pubkey.len() as u64)?;
        
        // Script
        writer.write_all(&self.script_pubkey)?;
        
        Ok(())
    }
}

impl Deserialize for TxOutput {
    fn deserialize<R: Read>(reader: &mut R) -> io::Result<Self> {
        // Value
        let mut value_bytes = [0u8; 8];
        reader.read_exact(&mut value_bytes)?;
        let value = u64::from_le_bytes(value_bytes);
        
        // Script length
        let script_len = read_compact_size(reader)?;
        
        // Script
        let mut script_pubkey = vec![0u8; script_len as usize];
        reader.read_exact(&mut script_pubkey)?;
        
        Ok(TxOutput {
            value,
            script_pubkey,
        })
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_compact_size() {
        let test_cases = vec![
            (0u64, vec![0x00]),
            (252u64, vec![0xFC]),
            (253u64, vec![0xFD, 0xFD, 0x00]),
            (0xFFFFu64, vec![0xFD, 0xFF, 0xFF]),
            (0x10000u64, vec![0xFE, 0x00, 0x00, 0x01, 0x00]),
        ];
        
        for (value, expected) in test_cases {
            let mut buf = Vec::new();
            write_compact_size(&mut buf, value).unwrap();
            assert_eq!(buf, expected);
            
            let mut reader = &buf[..];
            let read_value = read_compact_size(&mut reader).unwrap();
            assert_eq!(read_value, value);
        }
    }
}