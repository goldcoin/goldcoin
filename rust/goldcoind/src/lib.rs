// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! Goldcoin daemon library

pub mod config;
pub mod daemon;
pub mod logging;
pub mod shutdown;

pub use config::GoldcoindConfig;
pub use daemon::GoldcoinDaemon;