// Copyright (c) 2010-2016 The Bitcoin Core developers
// Copyright (c) 2013-2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "ui_interface.h"
#include "util.h"

#include <atomic>

CClientUIInterface uiInterface;

/**
 * Implementation of UISignalUtils::SignalDiagnostics static members
 * These track signal usage statistics for monitoring and debugging
 */
std::atomic<size_t> UISignalUtils::SignalDiagnostics::total_callbacks_registered{0};
std::atomic<size_t> UISignalUtils::SignalDiagnostics::total_signals_fired{0};
std::atomic<size_t> UISignalUtils::SignalDiagnostics::boost_signals_fired{0};
std::atomic<size_t> UISignalUtils::SignalDiagnostics::modern_callbacks_fired{0};

bool InitError(const std::string& str)
{
    LogPrintf("init error: %s\n", str);
    uiInterface.TriggerInitMessage("Error: " + str);
    uiInterface.TriggerThreadSafeMessageBox(str, "", CClientUIInterface::MSG_ERROR);
    return false;
}

void InitWarning(const std::string& str)
{
    LogPrintf("init warning: %s\n", str);
    uiInterface.TriggerInitMessage("Warning: " + str);
    uiInterface.TriggerThreadSafeMessageBox(str, "", CClientUIInterface::MSG_WARNING);
}

std::string AmountHighWarn(const std::string& optname)
{
    return strprintf(_("%s is set very high!"), optname);
}

std::string AmountErrMsg(const char* const optname, const std::string& strValue)
{
    return strprintf(_("Invalid amount for -%s=<amount>: '%s'"), optname, strValue);
}
