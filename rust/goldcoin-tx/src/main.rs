// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! Goldcoin Transaction Tool - Pure Rust Implementation
//! 
//! Utility for creating and manipulating Goldcoin transactions.
//! Maintains full compatibility with the C++23 implementation.

use std::collections::HashMap;
use std::path::PathBuf;

use anyhow::{Context, Result};
use clap::{Arg, Command, ArgMatches};
use serde_json::{Value, json};

mod transaction;
mod input;
mod output;
mod script;
mod signing;

use transaction::{Transaction, TxInput, TxOutput};
use script::Script;

const VERSION: &str = "0.17.0";
const COPYRIGHT: &str = "Copyright (C) 2013-2025 The Goldcoin Core developers";

fn main() -> Result<()> {
    let matches = build_cli().get_matches();
    
    // Show version if requested
    if matches.get_flag("version") {
        println!("Goldcoin Transaction Tool version v{}", VERSION);
        println!("{}", COPYRIGHT);
        println!("Pure Rust Implementation - Latest Rust");
        return Ok(());
    }
    
    // Parse input transaction
    let mut tx = parse_input_transaction(&matches)?;
    
    // Apply modifications based on command line arguments
    apply_modifications(&mut tx, &matches)?;
    
    // Output the result
    output_transaction(&tx, &matches)?;
    
    Ok(())
}

fn build_cli() -> Command {
    Command::new("goldcoin-tx")
        .version(VERSION)
        .about("Goldcoin Transaction Tool - Pure Rust Implementation")
        .long_about("Create and manipulate Goldcoin transactions")
        .arg(
            Arg::new("version")
                .long("version")
                .help("Print version information and exit")
                .action(clap::ArgAction::SetTrue)
        )
        .arg(
            Arg::new("create")
                .long("create")
                .help("Create a new empty transaction")
                .action(clap::ArgAction::SetTrue)
        )
        .arg(
            Arg::new("json")
                .long("json")
                .help("Output transaction in JSON format")
                .action(clap::ArgAction::SetTrue)
        )
        .arg(
            Arg::new("txid")
                .long("txid")
                .help("Output just the transaction ID")
                .action(clap::ArgAction::SetTrue)
        )
        .arg(
            Arg::new("hex")
                .value_name("HEX")
                .help("Input transaction as hex string")
                .index(1)
        )
        .arg(
            Arg::new("in")
                .long("in")
                .help("Add input: TXID:VOUT[:SEQUENCE]")
                .value_name("TXID:VOUT")
                .action(clap::ArgAction::Append)
        )
        .arg(
            Arg::new("out")
                .long("out")
                .help("Add output: ADDRESS:AMOUNT")
                .value_name("ADDRESS:AMOUNT")
                .action(clap::ArgAction::Append)
        )
        .arg(
            Arg::new("locktime")
                .long("locktime")
                .help("Set transaction locktime")
                .value_name("LOCKTIME")
        )
        .arg(
            Arg::new("replaceable")
                .long("replaceable")
                .help("Set RBF flag on inputs")
                .action(clap::ArgAction::SetTrue)
        )
        .arg(
            Arg::new("noinput")
                .long("noinput")
                .help("Remove input at index")
                .value_name("INDEX")
                .action(clap::ArgAction::Append)
        )
        .arg(
            Arg::new("nooutput")
                .long("nooutput")
                .help("Remove output at index")
                .value_name("INDEX")
                .action(clap::ArgAction::Append)
        )
        .arg(
            Arg::new("sign")
                .long("sign")
                .help("Sign transaction with private key file")
                .value_name("KEYFILE")
        )
        .arg(
            Arg::new("load")
                .long("load")
                .help("Load transaction from file")
                .value_name("FILE")
        )
        .arg(
            Arg::new("save")
                .long("save")
                .help("Save transaction to file")
                .value_name("FILE")
        )
}

fn parse_input_transaction(matches: &ArgMatches) -> Result<Transaction> {
    if matches.get_flag("create") {
        // Create new empty transaction
        Ok(Transaction::new())
    } else if let Some(filename) = matches.get_one::<String>("load") {
        // Load from file
        let content = std::fs::read_to_string(filename)
            .context(format!("Failed to read file: {}", filename))?;
        
        if content.trim().starts_with('{') {
            // JSON format
            let json: Value = serde_json::from_str(&content)
                .context("Failed to parse JSON transaction")?;
            Transaction::from_json(&json)
        } else {
            // Assume hex format
            Transaction::from_hex(content.trim())
        }
    } else if let Some(hex) = matches.get_one::<String>("hex") {
        // Parse from hex string
        Transaction::from_hex(hex)
    } else {
        // Read from stdin
        let mut input = String::new();
        std::io::stdin().read_line(&mut input)
            .context("Failed to read from stdin")?;
        
        let input = input.trim();
        if input.starts_with('{') {
            let json: Value = serde_json::from_str(input)
                .context("Failed to parse JSON from stdin")?;
            Transaction::from_json(&json)
        } else {
            Transaction::from_hex(input)
        }
    }
}

fn apply_modifications(tx: &mut Transaction, matches: &ArgMatches) -> Result<()> {
    // Add inputs
    if let Some(inputs) = matches.get_many::<String>("in") {
        for input_spec in inputs {
            let parts: Vec<&str> = input_spec.split(':').collect();
            if parts.len() < 2 {
                return Err(anyhow::anyhow!("Invalid input format: {}", input_spec));
            }
            
            let txid = parts[0];
            let vout: u32 = parts[1].parse()
                .context(format!("Invalid vout: {}", parts[1]))?;
            let sequence = if parts.len() > 2 {
                parts[2].parse().unwrap_or(0xfffffffe)
            } else {
                0xfffffffe // Default for RBF
            };
            
            let input = TxInput {
                previous_output: (hex::decode(txid)
                    .context("Invalid txid hex")?, vout),
                script_sig: Script::new(),
                sequence,
            };
            
            tx.add_input(input);
        }
    }
    
    // Add outputs
    if let Some(outputs) = matches.get_many::<String>("out") {
        for output_spec in outputs {
            let parts: Vec<&str> = output_spec.split(':').collect();
            if parts.len() != 2 {
                return Err(anyhow::anyhow!("Invalid output format: {}", output_spec));
            }
            
            let address = parts[0];
            let amount: f64 = parts[1].parse()
                .context(format!("Invalid amount: {}", parts[1]))?;
            
            // Convert amount to satoshis
            let amount_satoshis = (amount * 100_000_000.0) as u64;
            
            let output = TxOutput {
                value: amount_satoshis,
                script_pubkey: Script::from_address(address)?,
            };
            
            tx.add_output(output);
        }
    }
    
    // Set locktime
    if let Some(locktime_str) = matches.get_one::<String>("locktime") {
        let locktime: u32 = locktime_str.parse()
            .context("Invalid locktime")?;
        tx.set_locktime(locktime);
    }
    
    // Set RBF flag
    if matches.get_flag("replaceable") {
        tx.set_rbf_flag();
    }
    
    // Remove inputs
    if let Some(indices) = matches.get_many::<String>("noinput") {
        let mut indices: Vec<usize> = indices
            .map(|s| s.parse().context("Invalid input index"))
            .collect::<Result<Vec<_>>>()?;
        indices.sort_by(|a, b| b.cmp(a)); // Sort in reverse order
        
        for index in indices {
            tx.remove_input(index)?;
        }
    }
    
    // Remove outputs
    if let Some(indices) = matches.get_many::<String>("nooutput") {
        let mut indices: Vec<usize> = indices
            .map(|s| s.parse().context("Invalid output index"))
            .collect::<Result<Vec<_>>>()?;
        indices.sort_by(|a, b| b.cmp(a)); // Sort in reverse order
        
        for index in indices {
            tx.remove_output(index)?;
        }
    }
    
    // Sign transaction
    if let Some(keyfile) = matches.get_one::<String>("sign") {
        signing::sign_transaction(tx, keyfile)?;
    }
    
    Ok(())
}

fn output_transaction(tx: &Transaction, matches: &ArgMatches) -> Result<()> {
    if matches.get_flag("txid") {
        // Output just the transaction ID
        println!("{}", hex::encode(tx.txid()));
    } else if matches.get_flag("json") {
        // Output in JSON format
        let json = tx.to_json();
        println!("{}", serde_json::to_string_pretty(&json)?);
    } else {
        // Output as hex (default)
        println!("{}", tx.to_hex());
    }
    
    // Save to file if requested
    if let Some(filename) = matches.get_one::<String>("save") {
        let content = if matches.get_flag("json") {
            serde_json::to_string_pretty(&tx.to_json())?
        } else {
            tx.to_hex()
        };
        
        std::fs::write(filename, content)
            .context(format!("Failed to write to file: {}", filename))?;
        
        eprintln!("Transaction saved to {}", filename);
    }
    
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_cli_parsing() {
        let app = build_cli();
        let matches = app.try_get_matches_from(vec!["goldcoin-tx", "--create"]).unwrap();
        assert!(matches.get_flag("create"));
    }
    
    #[test]
    fn test_empty_transaction_creation() {
        let tx = Transaction::new();
        assert_eq!(tx.inputs().len(), 0);
        assert_eq!(tx.outputs().len(), 0);
        assert_eq!(tx.locktime(), 0);
    }
    
    #[test]
    fn test_transaction_hex_roundtrip() {
        let tx = Transaction::new();
        let hex = tx.to_hex();
        let parsed = Transaction::from_hex(&hex).unwrap();
        assert_eq!(parsed.to_hex(), hex);
    }
}