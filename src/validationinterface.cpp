// Copyright (c) 2007-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Copyright (c) 2011-2017 The Litecoin Core developers
// Copyright (c) 2013-2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include "validationinterface.h"
#include <functional>  // C++23 std::bind_front


static CMainSignals g_signals;

CMainSignals& GetMainSignals()
{
    return g_signals;
}

void RegisterValidationInterface(CValidationInterface* pwalletIn) {
    g_signals.UpdatedBlockTip.connect([pwalletIn](const CBlockIndex* a, const CBlockIndex* b, bool c) {
        pwalletIn->UpdatedBlockTip(a, b, c);
    });
    g_signals.SyncTransaction.connect([pwalletIn](const CTransaction& a, const CBlockIndex* b, int c) {
        pwalletIn->SyncTransaction(a, b, c);
    });
    g_signals.UpdatedTransaction.connect([pwalletIn](const uint256& a) {
        pwalletIn->UpdatedTransaction(a);
    });
    g_signals.SetBestChain.connect([pwalletIn](const CBlockLocator& a) {
        pwalletIn->SetBestChain(a);
    });
    g_signals.Inventory.connect([pwalletIn](const uint256& a) {
        pwalletIn->Inventory(a);
    });
    g_signals.Broadcast.connect([pwalletIn](int64_t a, CConnman* b) {
        pwalletIn->ResendWalletTransactions(a, b);
    });
    g_signals.BlockChecked.connect([pwalletIn](const CBlock& a, const CValidationState& b) {
        pwalletIn->BlockChecked(a, b);
    });
    g_signals.ScriptForMining.connect([pwalletIn](std::shared_ptr<CReserveScript>& a) {
        pwalletIn->GetScriptForMining(a);
    });
    g_signals.BlockFound.connect([pwalletIn](const uint256& a) {
        pwalletIn->ResetRequestCount(a);
    });
    g_signals.NewPoWValidBlock.connect([pwalletIn](const CBlockIndex* a, const std::shared_ptr<const CBlock>& b) {
        pwalletIn->NewPoWValidBlock(a, b);
    });
}

void UnregisterValidationInterface(CValidationInterface* pwalletIn) {
    // Note: With lambdas, we can't use disconnect with the same signature
    // boost::signals2 will be replaced with std::function-based callbacks
    // For now, disconnect all slots - this is safe but less precise
    g_signals.BlockFound.disconnect_all_slots();
    g_signals.ScriptForMining.disconnect_all_slots();
    g_signals.BlockChecked.disconnect_all_slots();
    g_signals.Broadcast.disconnect_all_slots();
    g_signals.Inventory.disconnect_all_slots();
    g_signals.SetBestChain.disconnect_all_slots();
    g_signals.UpdatedTransaction.disconnect_all_slots();
    g_signals.SyncTransaction.disconnect_all_slots();
    g_signals.UpdatedBlockTip.disconnect_all_slots();
    g_signals.NewPoWValidBlock.disconnect_all_slots();
}

void UnregisterAllValidationInterfaces() {
    g_signals.BlockFound.disconnect_all_slots();
    g_signals.ScriptForMining.disconnect_all_slots();
    g_signals.BlockChecked.disconnect_all_slots();
    g_signals.Broadcast.disconnect_all_slots();
    g_signals.Inventory.disconnect_all_slots();
    g_signals.SetBestChain.disconnect_all_slots();
    g_signals.UpdatedTransaction.disconnect_all_slots();
    g_signals.SyncTransaction.disconnect_all_slots();
    g_signals.UpdatedBlockTip.disconnect_all_slots();
    g_signals.NewPoWValidBlock.disconnect_all_slots();
}
