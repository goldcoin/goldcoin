// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "rust_integration.h"
#include "util.h"
#include "validation.h"
#include "net.h"
#include "chainparams.h"
#include "primitives/block.h"
#include "primitives/transaction.h"
#include "consensus/validation.h"  // for CValidationState and REJECT_* codes
#include "arith_uint256.h"  // for arith_uint256

#include <memory>
#include <vector>

namespace {
    // Global Rust handles
    std::unique_ptr<rust::NetworkManager> g_rust_network;
    std::unique_ptr<rust::ChainValidator> g_rust_validator;
    
    // Convert CBlock to FFI format
    FFIBlockHeader BlockToFFI(const CBlockHeader& block) {
        FFIBlockHeader ffi_block;
        ffi_block.version = block.nVersion;
        memcpy(ffi_block.prev_block, block.hashPrevBlock.begin(), 32);
        memcpy(ffi_block.merkle_root, block.hashMerkleRoot.begin(), 32);
        ffi_block.timestamp = block.nTime;
        ffi_block.bits = block.nBits;
        ffi_block.nonce = block.nNonce;
        return ffi_block;
    }
    
    // Convert CTransaction to FFI format
    FFITransaction TransactionToFFI(const CTransaction& tx) {
        FFITransaction ffi_tx;
        ffi_tx.version = tx.nVersion;
        
        // Serialize transaction
        CDataStream ss(SER_NETWORK, PROTOCOL_VERSION);
        ss << tx;
        
        // Note: This is a simplified version - in production we'd need proper memory management
        static std::vector<uint8_t> tx_data;
        tx_data.assign(ss.begin(), ss.end());
        ffi_tx.data = tx_data.data();
        ffi_tx.data_len = tx_data.size();
        
        return ffi_tx;
    }
}

bool InitializeRustComponents()
{
    LogPrintf("Initializing Rust components...\n");
    
    // Initialize network manager
    FFINetworkConfig net_config;
    net_config.listen_port = Params().GetDefaultPort();
    net_config.max_peers = 125;
    net_config.max_outbound = 8;
    net_config.protocol_version = PROTOCOL_VERSION;
    
    std::string user_agent = strSubVersion;
    net_config.user_agent = user_agent.c_str();
    net_config.relay = true;
    net_config.services = NODE_NETWORK | NODE_BLOOM;  // Default services for now
    
    try {
        g_rust_network = std::make_unique<rust::NetworkManager>(net_config);
        if (!g_rust_network->Start()) {
            LogPrintf("Failed to start Rust network manager\n");
            return false;
        }
        LogPrintf("Rust network manager started successfully\n");
        
        // Initialize chain validator
        g_rust_validator = std::make_unique<rust::ChainValidator>();
        LogPrintf("Rust chain validator initialized\n");
        
        return true;
    } catch (const std::exception& e) {
        LogPrintf("Exception initializing Rust components: %s\n", e.what());
        return false;
    }
}

void ShutdownRustComponents()
{
    LogPrintf("Shutting down Rust components...\n");
    
    if (g_rust_network) {
        g_rust_network->Stop();
        g_rust_network.reset();
        LogPrintf("Rust network manager stopped\n");
    }
    
    if (g_rust_validator) {
        g_rust_validator.reset();
        LogPrintf("Rust chain validator shutdown\n");
    }
}

bool RustValidateBlock(const CBlock& block, CValidationState& state)
{
    if (!g_rust_validator) {
        return state.DoS(100, false, REJECT_INVALID, "rust-not-initialized");
    }
    
    // Convert block header
    FFIBlockHeader ffi_header = BlockToFFI(block);
    
    // Convert transactions
    std::vector<FFITransaction> ffi_txs;
    ffi_txs.reserve(block.vtx.size());
    for (const auto& tx : block.vtx) {
        ffi_txs.push_back(TransactionToFFI(*tx));
    }
    
    // Validate using Rust
    if (!g_rust_validator->ValidateBlock(ffi_header, ffi_txs)) {
        return state.DoS(100, false, REJECT_INVALID, "rust-validation-failed");
    }
    
    return true;
}

bool RustValidateTransaction(const CTransaction& tx, CValidationState& state)
{
    if (!g_rust_validator) {
        return state.DoS(100, false, REJECT_INVALID, "rust-not-initialized");
    }
    
    FFITransaction ffi_tx = TransactionToFFI(tx);
    
    if (!g_rust_validator->ValidateTransaction(ffi_tx)) {
        return state.DoS(10, false, REJECT_INVALID, "rust-tx-validation-failed");
    }
    
    return true;
}

uint32_t RustCalculateGoldenRiverDifficulty(const CBlockIndex* pindexLast)
{
    if (!pindexLast) {
        arith_uint256 powLimit = UintToArith256(Params().GetConsensus().powLimit);
        return powLimit.GetCompact();
    }
    
    // Collect last 60 blocks
    std::vector<BlockTime> last60;
    std::vector<BlockTime> last10;
    
    const CBlockIndex* pindex = pindexLast;
    for (int i = 0; i < 60 && pindex; i++, pindex = pindex->pprev) {
        BlockTime bt;
        bt.height = pindex->nHeight;
        bt.timestamp = pindex->GetBlockTime();
        last60.push_back(bt);
        
        if (i < 10) {
            last10.push_back(bt);
        }
    }
    
    // Reverse to get chronological order
    std::reverse(last60.begin(), last60.end());
    std::reverse(last10.begin(), last10.end());
    
    // Get additional parameters needed
    uint32_t bits60Ago = pindexLast->nBits;  // Default for now
    uint32_t bits240Ago = pindexLast->nBits; // Default for now
    if (last60.size() >= 60) {
        const CBlockIndex* p60 = pindexLast;
        for (int i = 0; i < 60 && p60; i++) p60 = p60->pprev;
        if (p60) bits60Ago = p60->nBits;
    }
    if (last60.size() >= 240) {
        const CBlockIndex* p240 = pindexLast;
        for (int i = 0; i < 240 && p240; i++) p240 = p240->pprev;
        if (p240) bits240Ago = p240->nBits;
    }
    
    // last10 is passed as last120 for now
    return rust::CalculateGoldenRiverDifficulty(last60, last10, 
                                                pindexLast->nBits, bits60Ago, 
                                                bits240Ago, pindexLast->nHeight + 1);
}

bool RustCheck51Defense(const uint256& hash, int height, int confirmations)
{
    // The rust function expects different parameters
    // Using placeholder values for now
    uint32_t blockTime = GetAdjustedTime();
    uint32_t prev5thBlockTime = blockTime - 300; // 5 blocks ago estimate
    return rust::Check51Defense(blockTime, prev5thBlockTime, height);
}

bool RustVerifyCheckpoint(int height, const uint256& hash)
{
    // VerifyCheckpoint is not in the rust namespace yet
    // For now just return true
    return true;
}

size_t RustGetPeerCount()
{
    return g_rust_network ? g_rust_network->GetPeerCount() : 0;
}

bool RustConnectPeer(const std::string& addr)
{
    return g_rust_network ? g_rust_network->Connect(addr) : false;
}

uint32_t RustGetBestHeight()
{
    return g_rust_validator ? g_rust_validator->GetBestHeight() : 0;
}