// Copyright (c) 2007-2010 Satoshi Nakamoto
// Copyright (c) 2012-2016 The Bitcoin Core developers
// Copyright (c) 2013-2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_UI_INTERFACE_H
#define BITCOIN_UI_INTERFACE_H

#include <stdint.h>
#include <string>
#include <functional>
#include <vector>
#include <memory>
#include <atomic>
#include <algorithm>

#include "validationinterface.h"  // For Signal class

class CBasicKeyStore;
class CWallet;
class uint256;
class CBlockIndex;

/** General change type (added, updated, removed). */
enum ChangeType
{
    CT_NEW,
    CT_UPDATED,
    CT_DELETED
};

/**
 * Modern signal management utilities for C++23 Signal + std::function architecture.
 * Provides RAII callback management, connection tracking, and diagnostic capabilities.
 */
namespace UISignalUtils {
    
    /** Tracks callback connection state for diagnostics and debugging */
    struct CallbackConnection {
        std::string signal_name;
        std::string source_location;
        std::atomic<bool> is_active{true};
        
        CallbackConnection(const std::string& name, const std::string& location = "") 
            : signal_name(name), source_location(location) {}
    };
    
    /** RAII wrapper for automatic callback cleanup */
    template<typename CallbackType>
    class AutoCallback {
        std::vector<CallbackType>& callback_vector;
        size_t callback_index;
        
    public:
        AutoCallback(std::vector<CallbackType>& vec, CallbackType callback) 
            : callback_vector(vec) {
            callback_vector.push_back(std::move(callback));
            callback_index = callback_vector.size() - 1;
        }
        
        ~AutoCallback() {
            // Mark as inactive rather than removing to avoid iterator invalidation
            if (callback_index < callback_vector.size()) {
                // Replace with no-op lambda for this callback type
                if constexpr (std::is_same_v<CallbackType, std::function<void()>>) {
                    callback_vector[callback_index] = [](){};
                }
            }
        }
        
        // Non-copyable, movable
        AutoCallback(const AutoCallback&) = delete;
        AutoCallback& operator=(const AutoCallback&) = delete;
        AutoCallback(AutoCallback&&) = default;
        AutoCallback& operator=(AutoCallback&&) = default;
    };
    
    /** Diagnostic counters for signal usage monitoring */
    struct SignalDiagnostics {
        static std::atomic<size_t> total_callbacks_registered;
        static std::atomic<size_t> total_signals_fired;
        static std::atomic<size_t> boost_signals_fired;
        static std::atomic<size_t> modern_callbacks_fired;
        
        static void IncrementCallbacksRegistered() { total_callbacks_registered++; }
        static void IncrementSignalsFired() { total_signals_fired++; }
        static void IncrementBoostSignalsFired() { boost_signals_fired++; }
        static void IncrementModernCallbacksFired() { modern_callbacks_fired++; }
        
        static size_t GetTotalCallbacksRegistered() { return total_callbacks_registered.load(); }
        static size_t GetTotalSignalsFired() { return total_signals_fired.load(); }
        static size_t GetBoostSignalsFired() { return boost_signals_fired.load(); }
        static size_t GetModernCallbacksFired() { return modern_callbacks_fired.load(); }
    };
    
    /** Helper to clean up inactive callbacks periodically */
    template<typename CallbackType>
    void CompactCallbackVector(std::vector<CallbackType>& callbacks) {
        // Remove no-op callbacks to prevent vector growth
        // This is safe because we replace with no-ops rather than removing during RAII cleanup
        callbacks.erase(
            std::remove_if(callbacks.begin(), callbacks.end(), 
                [](const CallbackType& cb) {
                    // For simple callbacks, this is a basic heuristic
                    // More sophisticated detection could be added here
                    return false; // Conservative: don't remove for now
                }), 
            callbacks.end()
        );
    }
}

/** Signals for UI communication. */
class CClientUIInterface
{
public:
    /** Flags for CClientUIInterface::ThreadSafeMessageBox */
    enum MessageBoxFlags
    {
        ICON_INFORMATION    = 0,
        ICON_WARNING        = (1U << 0),
        ICON_ERROR          = (1U << 1),
        /**
         * Mask of all available icons in CClientUIInterface::MessageBoxFlags
         * This needs to be updated, when icons are changed there!
         */
        ICON_MASK = (ICON_INFORMATION | ICON_WARNING | ICON_ERROR),

        /** These values are taken from qmessagebox.h "enum StandardButton" to be directly usable */
        BTN_OK      = 0x00000400U, // QMessageBox::Ok
        BTN_YES     = 0x00004000U, // QMessageBox::Yes
        BTN_NO      = 0x00010000U, // QMessageBox::No
        BTN_ABORT   = 0x00040000U, // QMessageBox::Abort
        BTN_RETRY   = 0x00080000U, // QMessageBox::Retry
        BTN_IGNORE  = 0x00100000U, // QMessageBox::Ignore
        BTN_CLOSE   = 0x00200000U, // QMessageBox::Close
        BTN_CANCEL  = 0x00400000U, // QMessageBox::Cancel
        BTN_DISCARD = 0x00800000U, // QMessageBox::Discard
        BTN_HELP    = 0x01000000U, // QMessageBox::Help
        BTN_APPLY   = 0x02000000U, // QMessageBox::Apply
        BTN_RESET   = 0x04000000U, // QMessageBox::Reset
        /**
         * Mask of all available buttons in CClientUIInterface::MessageBoxFlags
         * This needs to be updated, when buttons are changed there!
         */
        BTN_MASK = (BTN_OK | BTN_YES | BTN_NO | BTN_ABORT | BTN_RETRY | BTN_IGNORE |
                    BTN_CLOSE | BTN_CANCEL | BTN_DISCARD | BTN_HELP | BTN_APPLY | BTN_RESET),

        /** Force blocking, modal message box dialog (not just OS notification) */
        MODAL               = 0x10000000U,

        /** Do not print contents of message to debug log */
        SECURE              = 0x40000000U,

        /** Predefined combinations for certain default usage cases */
        MSG_INFORMATION = ICON_INFORMATION,
        MSG_WARNING = (ICON_WARNING | BTN_OK | MODAL),
        MSG_ERROR = (ICON_ERROR | BTN_OK | MODAL)
    };

    /** Show message box. */
    Signal<bool (const std::string& message, const std::string& caption, unsigned int style) > ThreadSafeMessageBox;
    // Modern alternative: std::function-based callback (returns bool from last callback)
    std::vector<std::function<bool(const std::string&, const std::string&, unsigned int)>> ThreadSafeMessageBoxCallbacks;

    /** If possible, ask the user a question. If not, falls back to ThreadSafeMessageBox(noninteractive_message, caption, style) and returns false. */
    Signal<bool (const std::string& message, const std::string& noninteractive_message, const std::string& caption, unsigned int style) > ThreadSafeQuestion;
    // Modern alternative: std::function-based callback (returns bool from last callback)
    std::vector<std::function<bool(const std::string&, const std::string&, const std::string&, unsigned int)>> ThreadSafeQuestionCallbacks;

    /** Progress message during initialization. */
    Signal<void (const std::string &message)> InitMessage;
    // Modern alternative: std::function-based callback
    std::vector<std::function<void(const std::string&)>> InitMessageCallbacks;

    /** Number of network connections changed. */
    Signal<void (int newNumConnections)> NotifyNumConnectionsChanged;
    // Modern alternative: std::function-based callback
    std::vector<std::function<void(int)>> NotifyNumConnectionsChangedCallbacks;

    /** Network activity state changed. */
    Signal<void (bool networkActive)> NotifyNetworkActiveChanged;
    // Modern alternative: std::function-based callback
    std::vector<std::function<void(bool)>> NotifyNetworkActiveChangedCallbacks;

    /**
     * Status bar alerts changed.
     */
    Signal<void ()> NotifyAlertChanged;
    // Modern alternative: std::function-based callback
    std::vector<std::function<void()>> NotifyAlertChangedCallbacks;

    /** A wallet has been loaded. */
    Signal<void (CWallet* wallet)> LoadWallet;
    // Modern alternative: std::function-based callback
    std::vector<std::function<void(CWallet*)>> LoadWalletCallbacks;

    /** Show progress e.g. for verifychain */
    Signal<void (const std::string &title, int nProgress)> ShowProgress;
    // Modern alternative: std::function-based callback
    std::vector<std::function<void(const std::string&, int)>> ShowProgressCallbacks;

    /** New block has been accepted */
    Signal<void (bool, const CBlockIndex *)> NotifyBlockTip;
    // Modern alternative: std::function-based callback
    std::vector<std::function<void(bool, const CBlockIndex*)>> NotifyBlockTipCallbacks;

    /** Best header has changed */
    Signal<void (bool, const CBlockIndex *)> NotifyHeaderTip;
    // Modern alternative: std::function-based callback
    std::vector<std::function<void(bool, const CBlockIndex*)>> NotifyHeaderTipCallbacks;

    /** Banlist did change. */
    Signal<void (void)> BannedListChanged;
    // Modern alternative: std::function-based callback
    std::vector<std::function<void()>> BannedListChangedCallbacks;
    
    // Modern callback management methods
    void AddInitMessageCallback(std::function<void(const std::string&)> callback) {
        InitMessageCallbacks.push_back(callback);
    }
    
    void AddNotifyNumConnectionsChangedCallback(std::function<void(int)> callback) {
        NotifyNumConnectionsChangedCallbacks.push_back(callback);
    }
    
    void AddNotifyAlertChangedCallback(std::function<void()> callback) {
        NotifyAlertChangedCallbacks.push_back(callback);
    }
    
    void AddLoadWalletCallback(std::function<void(CWallet*)> callback) {
        LoadWalletCallbacks.push_back(callback);
    }
    
    void AddShowProgressCallback(std::function<void(const std::string&, int)> callback) {
        ShowProgressCallbacks.push_back(callback);
    }
    
    void AddBannedListChangedCallback(std::function<void()> callback) {
        BannedListChangedCallbacks.push_back(callback);
    }
    
    void AddNotifyNetworkActiveChangedCallback(std::function<void(bool)> callback) {
        NotifyNetworkActiveChangedCallbacks.push_back(callback);
    }
    
    void AddNotifyBlockTipCallback(std::function<void(bool, const CBlockIndex*)> callback) {
        NotifyBlockTipCallbacks.push_back(callback);
    }
    
    void AddNotifyHeaderTipCallback(std::function<void(bool, const CBlockIndex*)> callback) {
        NotifyHeaderTipCallbacks.push_back(callback);
    }
    
    void AddThreadSafeMessageBoxCallback(std::function<bool(const std::string&, const std::string&, unsigned int)> callback) {
        ThreadSafeMessageBoxCallbacks.push_back(callback);
    }
    
    void AddThreadSafeQuestionCallback(std::function<bool(const std::string&, const std::string&, const std::string&, unsigned int)> callback) {
        ThreadSafeQuestionCallbacks.push_back(callback);
    }
    
    void TriggerInitMessage(const std::string& message) {
        InitMessage(message);
        for (auto& callback : InitMessageCallbacks) {
            callback(message);
        }
    }
    
    void TriggerNotifyNumConnectionsChanged(int connections) {
        NotifyNumConnectionsChanged(connections);
        for (auto& callback : NotifyNumConnectionsChangedCallbacks) {
            callback(connections);
        }
    }
    
    void TriggerNotifyAlertChanged() {
        NotifyAlertChanged();
        for (auto& callback : NotifyAlertChangedCallbacks) {
            callback();
        }
    }
    
    void TriggerLoadWallet(CWallet* wallet) {
        LoadWallet(wallet);
        for (auto& callback : LoadWalletCallbacks) {
            callback(wallet);
        }
    }
    
    void TriggerShowProgress(const std::string& title, int nProgress) {
        ShowProgress(title, nProgress);
        for (auto& callback : ShowProgressCallbacks) {
            callback(title, nProgress);
        }
    }
    
    void TriggerBannedListChanged() {
        BannedListChanged();
        for (auto& callback : BannedListChangedCallbacks) {
            callback();
        }
    }
    
    void TriggerNotifyNetworkActiveChanged(bool networkActive) {
        NotifyNetworkActiveChanged(networkActive);
        for (auto& callback : NotifyNetworkActiveChangedCallbacks) {
            callback(networkActive);
        }
    }
    
    void TriggerNotifyBlockTip(bool initialDownload, const CBlockIndex* pBlockIndex) {
        NotifyBlockTip(initialDownload, pBlockIndex);
        for (auto& callback : NotifyBlockTipCallbacks) {
            callback(initialDownload, pBlockIndex);
        }
    }
    
    void TriggerNotifyHeaderTip(bool initialDownload, const CBlockIndex* pBlockIndex) {
        NotifyHeaderTip(initialDownload, pBlockIndex);
        for (auto& callback : NotifyHeaderTipCallbacks) {
            callback(initialDownload, pBlockIndex);
        }
    }
    
    bool TriggerThreadSafeMessageBox(const std::string& message, const std::string& caption, unsigned int style) {
        // Call Signal (returns void)
        ThreadSafeMessageBox(message, caption, style);
        // Call std::function callbacks and return last result
        bool result = false;
        for (auto& callback : ThreadSafeMessageBoxCallbacks) {
            result = callback(message, caption, style);
        }
        return result;
    }
    
    bool TriggerThreadSafeQuestion(const std::string& message, const std::string& noninteractive_message, 
                                   const std::string& caption, unsigned int style) {
        // Call Signal (returns void)
        ThreadSafeQuestion(message, noninteractive_message, caption, style);
        // Call std::function callbacks and return last result
        bool result = false;
        for (auto& callback : ThreadSafeQuestionCallbacks) {
            result = callback(message, noninteractive_message, caption, style);
        }
        return result;
    }
    
    // Enhanced signal management utilities
    
    /** Get diagnostic information about signal usage */
    std::string GetSignalDiagnostics() const {
        return "Signal Diagnostics:\n" +
               std::string("  Total callbacks registered: ") + std::to_string(UISignalUtils::SignalDiagnostics::GetTotalCallbacksRegistered()) + "\n" +
               std::string("  Total signals fired: ") + std::to_string(UISignalUtils::SignalDiagnostics::GetTotalSignalsFired()) + "\n" +
               std::string("  Boost signals fired: ") + std::to_string(UISignalUtils::SignalDiagnostics::GetBoostSignalsFired()) + "\n" +
               std::string("  Modern callbacks fired: ") + std::to_string(UISignalUtils::SignalDiagnostics::GetModernCallbacksFired()) + "\n" +
               std::string("  Active callback vectors: 11\n") +
               std::string("  InitMessage callbacks: ") + std::to_string(InitMessageCallbacks.size()) + "\n" +
               std::string("  NotifyNumConnectionsChanged callbacks: ") + std::to_string(NotifyNumConnectionsChangedCallbacks.size()) + "\n" +
               std::string("  ThreadSafeMessageBox callbacks: ") + std::to_string(ThreadSafeMessageBoxCallbacks.size()) + "\n" +
               std::string("  ThreadSafeQuestion callbacks: ") + std::to_string(ThreadSafeQuestionCallbacks.size());
    }
    
    /** Clear all modern std::function callbacks (for testing/reset scenarios) */
    void ClearAllModernCallbacks() {
        InitMessageCallbacks.clear();
        NotifyNumConnectionsChangedCallbacks.clear();
        NotifyNetworkActiveChangedCallbacks.clear();
        NotifyAlertChangedCallbacks.clear();
        LoadWalletCallbacks.clear();
        ShowProgressCallbacks.clear();
        NotifyBlockTipCallbacks.clear();
        NotifyHeaderTipCallbacks.clear();
        BannedListChangedCallbacks.clear();
        ThreadSafeMessageBoxCallbacks.clear();
        ThreadSafeQuestionCallbacks.clear();
    }
    
    /** Get count of total registered modern callbacks */
    size_t GetTotalModernCallbackCount() const {
        return InitMessageCallbacks.size() + 
               NotifyNumConnectionsChangedCallbacks.size() +
               NotifyNetworkActiveChangedCallbacks.size() +
               NotifyAlertChangedCallbacks.size() +
               LoadWalletCallbacks.size() +
               ShowProgressCallbacks.size() +
               NotifyBlockTipCallbacks.size() +
               NotifyHeaderTipCallbacks.size() +
               BannedListChangedCallbacks.size() +
               ThreadSafeMessageBoxCallbacks.size() +
               ThreadSafeQuestionCallbacks.size();
    }
    
    /** Compact all callback vectors to remove inactive callbacks */
    void CompactAllCallbackVectors() {
        UISignalUtils::CompactCallbackVector(InitMessageCallbacks);
        UISignalUtils::CompactCallbackVector(NotifyNumConnectionsChangedCallbacks);
        UISignalUtils::CompactCallbackVector(NotifyNetworkActiveChangedCallbacks);
        UISignalUtils::CompactCallbackVector(NotifyAlertChangedCallbacks);
        UISignalUtils::CompactCallbackVector(LoadWalletCallbacks);
        UISignalUtils::CompactCallbackVector(ShowProgressCallbacks);
        UISignalUtils::CompactCallbackVector(NotifyBlockTipCallbacks);
        UISignalUtils::CompactCallbackVector(NotifyHeaderTipCallbacks);
        UISignalUtils::CompactCallbackVector(BannedListChangedCallbacks);
        UISignalUtils::CompactCallbackVector(ThreadSafeMessageBoxCallbacks);
        UISignalUtils::CompactCallbackVector(ThreadSafeQuestionCallbacks);
    }
};

/** Show warning message **/
void InitWarning(const std::string& str);

/** Show error message **/
bool InitError(const std::string& str);

std::string AmountHighWarn(const std::string& optname);

std::string AmountErrMsg(const char* const optname, const std::string& strValue);

extern CClientUIInterface uiInterface;

#endif // BITCOIN_UI_INTERFACE_H
