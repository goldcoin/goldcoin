// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! Command definitions and help text for goldcoin-cli

use std::collections::HashMap;

pub struct CommandInfo {
    pub description: String,
    pub usage: String,
    pub examples: Vec<String>,
}

pub fn get_command_help() -> HashMap<&'static str, CommandInfo> {
    let mut commands = HashMap::new();
    
    commands.insert("getblockchaininfo", CommandInfo {
        description: "Returns an object containing various state info regarding blockchain processing.".to_string(),
        usage: "getblockchaininfo".to_string(),
        examples: vec!["goldcoin-cli getblockchaininfo".to_string()],
    });
    
    commands.insert("getbestblockhash", CommandInfo {
        description: "Returns the hash of the best (tip) block in the most-work fully-validated chain.".to_string(),
        usage: "getbestblockhash".to_string(),
        examples: vec!["goldcoin-cli getbestblockhash".to_string()],
    });
    
    commands.insert("getblockcount", CommandInfo {
        description: "Returns the number of blocks in the best valid block chain.".to_string(),
        usage: "getblockcount".to_string(),
        examples: vec!["goldcoin-cli getblockcount".to_string()],
    });
    
    commands.insert("getblock", CommandInfo {
        description: "Returns data about a block by hash.".to_string(),
        usage: "getblock \"blockhash\" [verbosity]".to_string(),
        examples: vec![
            "goldcoin-cli getblock \"00000000000000000001234567890abcdef\"".to_string(),
            "goldcoin-cli getblock \"00000000000000000001234567890abcdef\" 2".to_string(),
        ],
    });
    
    commands.insert("getpeerinfo", CommandInfo {
        description: "Returns data about each connected network node.".to_string(),
        usage: "getpeerinfo".to_string(),
        examples: vec!["goldcoin-cli getpeerinfo".to_string()],
    });
    
    commands.insert("getmempoolinfo", CommandInfo {
        description: "Returns details on the active state of the memory pool.".to_string(),
        usage: "getmempoolinfo".to_string(),
        examples: vec!["goldcoin-cli getmempoolinfo".to_string()],
    });
    
    commands.insert("help", CommandInfo {
        description: "List all commands, or get help for a specified command.".to_string(),
        usage: "help [\"command\"]".to_string(),
        examples: vec![
            "goldcoin-cli help".to_string(),
            "goldcoin-cli help getblockchaininfo".to_string(),
        ],
    });
    
    // Custom Goldcoin commands
    commands.insert("getdefenseinfo", CommandInfo {
        description: "Returns information about Goldcoin's 51% Defense System.".to_string(),
        usage: "getdefenseinfo".to_string(),
        examples: vec!["goldcoin-cli getdefenseinfo".to_string()],
    });
    
    commands.insert("getgoldenriverinfo", CommandInfo {
        description: "Returns information about Goldcoin's Golden River Difficulty Algorithm.".to_string(),
        usage: "getgoldenriverinfo".to_string(),
        examples: vec!["goldcoin-cli getgoldenriverinfo".to_string()],
    });
    
    commands
}