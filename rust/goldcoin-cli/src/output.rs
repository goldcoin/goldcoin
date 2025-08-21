// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! Output formatting for goldcoin-cli

use colored::*;
use serde_json::{Value, Map};

#[derive(Debug, Clone, Copy)]
pub enum OutputFormat {
    Json,
    Pretty,
}

pub fn display_result(value: &Value, format: OutputFormat) {
    match format {
        OutputFormat::Json => {
            println!("{}", serde_json::to_string_pretty(value).unwrap_or_else(|_| "null".to_string()));
        }
        OutputFormat::Pretty => {
            display_pretty(value, 0);
        }
    }
}

fn display_pretty(value: &Value, indent: usize) {
    let indent_str = "  ".repeat(indent);
    
    match value {
        Value::Null => println!("{}null", indent_str),
        Value::Bool(b) => println!("{}{}", indent_str, if *b { "true".green() } else { "false".red() }),
        Value::Number(n) => println!("{}{}", indent_str, n.to_string().yellow()),
        Value::String(s) => {
            if s.len() == 64 && s.chars().all(|c| c.is_ascii_hexdigit()) {
                // Likely a hash
                println!("{}{}", indent_str, s.blue());
            } else {
                println!("{}{}", indent_str, s);
            }
        }
        Value::Array(arr) => {
            if arr.is_empty() {
                println!("{}[]", indent_str);
            } else {
                println!("{}[", indent_str);
                for (i, item) in arr.iter().enumerate() {
                    print!("{}{}: ", "  ".repeat(indent + 1), i.to_string().cyan());
                    display_pretty(item, indent + 1);
                }
                println!("{}]", indent_str);
            }
        }
        Value::Object(obj) => {
            if obj.is_empty() {
                println!("{}{{}}", indent_str);
            } else {
                display_object(obj, indent);
            }
        }
    }
}

fn display_object(obj: &Map<String, Value>, indent: usize) {
    let indent_str = "  ".repeat(indent);
    
    // Special formatting for common objects
    if obj.contains_key("blocks") && obj.contains_key("chain") {
        display_blockchain_info(obj, indent);
    } else if obj.contains_key("id") && obj.contains_key("addr") {
        display_peer_info(obj, indent);
    } else if obj.contains_key("txid") && obj.contains_key("amount") {
        display_transaction(obj, indent);
    } else {
        // Generic object display
        println!("{}{{", indent_str);
        for (key, value) in obj {
            print!("{}{}: ", "  ".repeat(indent + 1), key.green().bold());
            match value {
                Value::String(s) if s.len() < 50 => println!("{}", s),
                Value::Number(n) => println!("{}", n.to_string().yellow()),
                Value::Bool(b) => println!("{}", if *b { "true".green() } else { "false".red() }),
                _ => {
                    println!();
                    display_pretty(value, indent + 1);
                }
            }
        }
        println!("{}}}", indent_str);
    }
}

fn display_blockchain_info(obj: &Map<String, Value>, indent: usize) {
    let indent_str = "  ".repeat(indent);
    println!("{}📊 {}", indent_str, "Blockchain Information".bold().green());
    
    if let Some(Value::String(chain)) = obj.get("chain") {
        println!("{}  Chain: {}", indent_str, chain.cyan());
    }
    
    if let Some(Value::Number(blocks)) = obj.get("blocks") {
        println!("{}  Blocks: {}", indent_str, blocks.to_string().yellow());
    }
    
    if let Some(Value::Number(difficulty)) = obj.get("difficulty") {
        println!("{}  Difficulty: {}", indent_str, format!("{:.2}", difficulty.as_f64().unwrap_or(0.0)).yellow());
    }
    
    if let Some(Value::String(hash)) = obj.get("bestblockhash") {
        println!("{}  Best Block: {}", indent_str, hash.blue());
    }
    
    if let Some(Value::Number(progress)) = obj.get("verificationprogress") {
        let pct = progress.as_f64().unwrap_or(0.0) * 100.0;
        println!("{}  Sync Progress: {:.2}%", indent_str, pct.to_string().yellow());
    }
}

fn display_peer_info(obj: &Map<String, Value>, indent: usize) {
    let indent_str = "  ".repeat(indent);
    println!("{}🌐 {}", indent_str, "Peer Information".bold().cyan());
    
    if let Some(Value::Number(id)) = obj.get("id") {
        println!("{}  ID: {}", indent_str, id.to_string().yellow());
    }
    
    if let Some(Value::String(addr)) = obj.get("addr") {
        println!("{}  Address: {}", indent_str, addr.cyan());
    }
    
    if let Some(Value::String(subver)) = obj.get("subver") {
        println!("{}  Version: {}", indent_str, subver);
    }
    
    if let Some(Value::Bool(inbound)) = obj.get("inbound") {
        let direction = if *inbound { "Inbound" } else { "Outbound" };
        println!("{}  Direction: {}", indent_str, direction);
    }
}

fn display_transaction(obj: &Map<String, Value>, indent: usize) {
    let indent_str = "  ".repeat(indent);
    println!("{}💰 {}", indent_str, "Transaction".bold().yellow());
    
    if let Some(Value::String(txid)) = obj.get("txid") {
        println!("{}  TXID: {}", indent_str, txid.blue());
    }
    
    if let Some(Value::Number(amount)) = obj.get("amount") {
        println!("{}  Amount: {} GLC", indent_str, amount.to_string().green());
    }
    
    if let Some(Value::Number(confirmations)) = obj.get("confirmations") {
        println!("{}  Confirmations: {}", indent_str, confirmations.to_string().yellow());
    }
    
    if let Some(Value::String(address)) = obj.get("address") {
        println!("{}  Address: {}", indent_str, address.cyan());
    }
}