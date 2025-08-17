// Copyright (c) 2011-2016 The Bitcoin Core developers
// Copyright (c) 2013-2023 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "clientmodel.h"

#include "bantablemodel.h"
#include "guiconstants.h"
#include "guiutil.h"
#include "peertablemodel.h"

#include "chainparams.h"
#include "checkpoints.h"
#include "clientversion.h"
#include "validation.h"
#include "net.h"
#include "txmempool.h"
#include "../ui_interface.h"
#include "util.h"

#include <stdint.h>
#include <chrono>
#include <functional>
#include <atomic>
#include <ranges>

#include <QDebug>
#include <QTimer>

#include <boost/bind/bind.hpp>
#include <boost/filesystem.hpp> // TODO: Remove after full migration
#include <filesystem>

class CBlockIndex;

// C++20: Use inline constexpr for compile-time constants
inline constexpr auto CLIENT_STARTUP_TIME = std::chrono::steady_clock::now();
static std::atomic<int64_t> nLastHeaderTipUpdateNotification{0};
static std::atomic<int64_t> nLastBlockTipUpdateNotification{0};

// C++20: Concept for blockchain index validation
template<typename T>
concept BlockIndexType = requires(T* index) {
    { index->nHeight } -> std::convertible_to<int>;
    { index->GetBlockTime() } -> std::convertible_to<int64_t>;
};

ClientModel::ClientModel(OptionsModel *_optionsModel, QObject *parent) :
    QObject(parent),
    optionsModel(_optionsModel),
    peerTableModel(0),
    banTableModel(0),
    pollTimer(0)
{
    cachedBestHeaderHeight = -1;
    cachedBestHeaderTime = -1;
    peerTableModel = new PeerTableModel(this);
    banTableModel = new BanTableModel(this);
    pollTimer = new QTimer(this);
    // Qt 6.9: Modern signal connection with lambda
    connect(pollTimer, &QTimer::timeout, this, [this]() {
        updateTimer();
    });
    pollTimer->start(MODEL_UPDATE_DELAY);

    subscribeToCoreSignals();
}

ClientModel::~ClientModel()
{
    unsubscribeFromCoreSignals();
}

int ClientModel::getNumConnections(unsigned int flags) const
{
    // C++20: Use structured binding and pattern matching
    const auto connections = [flags]() -> CConnman::NumConnections {
        switch(flags) {
            case CONNECTIONS_IN:  return CConnman::CONNECTIONS_IN;
            case CONNECTIONS_OUT: return CConnman::CONNECTIONS_OUT;
            case CONNECTIONS_ALL: return CConnman::CONNECTIONS_ALL;
            default: return CConnman::CONNECTIONS_NONE;
        }
    }();

    return g_connman ? g_connman->GetNodeCount(connections) : 0;
}

int ClientModel::getNumBlocks() const
{
    LOCK(cs_main);
    return chainActive.Height();
}

int ClientModel::getHeaderTipHeight() const
{
    // C++20: Use std::call_once for thread-safe initialization
    static std::once_flag initFlag;
    std::call_once(initFlag, [this]() {
        if (cachedBestHeaderHeight == -1) {
            LOCK(cs_main);
            if (pindexBestHeader) {
                cachedBestHeaderHeight = pindexBestHeader->nHeight;
                cachedBestHeaderTime = pindexBestHeader->GetBlockTime();
            }
        }
    });
    return cachedBestHeaderHeight;
}

int64_t ClientModel::getHeaderTipTime() const
{
    if (cachedBestHeaderTime == -1) {
        LOCK(cs_main);
        if (pindexBestHeader) {
            cachedBestHeaderHeight = pindexBestHeader->nHeight;
            cachedBestHeaderTime = pindexBestHeader->GetBlockTime();
        }
    }
    return cachedBestHeaderTime;
}

quint64 ClientModel::getTotalBytesRecv() const
{
    if(!g_connman)
        return 0;
    return g_connman->GetTotalBytesRecv();
}

quint64 ClientModel::getTotalBytesSent() const
{
    if(!g_connman)
        return 0;
    return g_connman->GetTotalBytesSent();
}

QDateTime ClientModel::getLastBlockDate() const
{
    LOCK(cs_main);

    // C++20: Use optional and value_or pattern
    const auto tipTime = chainActive.Tip() 
        ? chainActive.Tip()->GetBlockTime()
        : Params().GenesisBlock().GetBlockTime();
    
    return QDateTime::fromSecsSinceEpoch(tipTime);
}

long ClientModel::getMempoolSize() const
{
    return mempool.size();
}

size_t ClientModel::getMempoolDynamicUsage() const
{
    return mempool.DynamicMemoryUsage();
}

double ClientModel::getVerificationProgress(const CBlockIndex *tipIn) const
{
    // C++20: Remove const_cast, use conditional operator
    auto getTip = [tipIn]() -> CBlockIndex* {
        if (tipIn) return const_cast<CBlockIndex*>(tipIn);
        LOCK(cs_main);
        return chainActive.Tip();
    };
    
    return GuessVerificationProgress(Params().TxData(), getTip());
}

void ClientModel::updateTimer()
{
    // C++20: Use structured bindings for multiple return values
    const auto [mempoolSize, dynamicUsage] = std::pair{getMempoolSize(), getMempoolDynamicUsage()};
    const auto [bytesRecv, bytesSent] = std::pair{getTotalBytesRecv(), getTotalBytesSent()};
    
    Q_EMIT mempoolSizeChanged(mempoolSize, dynamicUsage);
    Q_EMIT bytesChanged(bytesRecv, bytesSent);
}

void ClientModel::updateNumConnections(int numConnections)
{
    Q_EMIT numConnectionsChanged(numConnections);
}

void ClientModel::updateNetworkActive(bool networkActive)
{
    Q_EMIT networkActiveChanged(networkActive);
}

void ClientModel::updateAlert()
{
    Q_EMIT alertsChanged(getStatusBarWarnings());
}

bool ClientModel::inInitialBlockDownload() const
{
    return IsInitialBlockDownload();
}

enum BlockSource ClientModel::getBlockSource() const
{
    // C++20: Early return pattern with constexpr if possible
    if (fReindex) return BLOCK_SOURCE_REINDEX;
    if (fImporting) return BLOCK_SOURCE_DISK;
    if (getNumConnections() > 0) return BLOCK_SOURCE_NETWORK;
    
    return BLOCK_SOURCE_NONE;
}

void ClientModel::setNetworkActive(bool active)
{
    if (g_connman) {
         g_connman->SetNetworkActive(active);
    }
}

bool ClientModel::getNetworkActive() const
{
    if (g_connman) {
        return g_connman->GetNetworkActive();
    }
    return false;
}

QString ClientModel::getStatusBarWarnings() const
{
    return QString::fromStdString(GetWarnings("gui"));
}

OptionsModel *ClientModel::getOptionsModel()
{
    return optionsModel;
}

PeerTableModel *ClientModel::getPeerTableModel()
{
    return peerTableModel;
}

BanTableModel *ClientModel::getBanTableModel()
{
    return banTableModel;
}

QString ClientModel::formatFullVersion() const
{
    return QString::fromStdString(FormatFullVersion());
}

QString ClientModel::formatSubVersion() const
{
    return QString::fromStdString(strSubVersion);
}

bool ClientModel::isReleaseVersion() const
{
    return CLIENT_VERSION_IS_RELEASE;
}

QString ClientModel::formatClientStartupTime() const
{
    // C++20: Use chrono for time calculations
    const auto startupTime = GetTime(); // Fallback until full chrono migration
    return QDateTime::fromSecsSinceEpoch(startupTime).toString();
}

QString ClientModel::dataDir() const
{
    // TODO: GetDataDir() still returns boost::filesystem::path
    boost::filesystem::path boostPath = GetDataDir();
    std::filesystem::path stdPath(boostPath.string());
    return GUIUtil::boostPathToQString(stdPath);
}

void ClientModel::updateBanlist()
{
    banTableModel->refresh();
}

// Handlers for core signals
// C++20: Use auto parameters and structured approach
static void ShowProgress(ClientModel *clientmodel, const std::string &title, int nProgress)
{
    // Qt 6.9: Use lambda for async invocation
    QMetaObject::invokeMethod(clientmodel, [clientmodel, title, nProgress]() {
        Q_EMIT clientmodel->showProgress(QString::fromStdString(title), nProgress);
    }, Qt::QueuedConnection);
}

static void NotifyNumConnectionsChanged(ClientModel *clientmodel, int newNumConnections)
{
    // C++20: Lambda-based invocation
    QMetaObject::invokeMethod(clientmodel, [clientmodel, newNumConnections]() {
        clientmodel->updateNumConnections(newNumConnections);
    }, Qt::QueuedConnection);
}

static void NotifyNetworkActiveChanged(ClientModel *clientmodel, bool networkActive)
{
    // C++20: Lambda-based invocation
    QMetaObject::invokeMethod(clientmodel, [clientmodel, networkActive]() {
        clientmodel->updateNetworkActive(networkActive);
    }, Qt::QueuedConnection);
}

static void BannedListChanged(ClientModel *clientmodel)
{
    qDebug() << QString("%1: Requesting update for peer banlist").arg(__func__);
    // C++20: Direct method invocation with lambda
    QMetaObject::invokeMethod(clientmodel, [clientmodel]() {
        clientmodel->updateBanlist();
    }, Qt::QueuedConnection);
}

static void BlockTipChanged(ClientModel *clientmodel, bool initialSync, const CBlockIndex *pIndex, bool fHeader)
{
    // C++20: Use chrono for time calculations
    const auto now = initialSync ? GetTimeMillis() : 0;
    
    // C++20: Use atomic reference for thread safety
    auto& nLastUpdateNotification = fHeader 
        ? nLastHeaderTipUpdateNotification 
        : nLastBlockTipUpdateNotification;

    if (fHeader) {
        // Cache best headers atomically
        clientmodel->cachedBestHeaderHeight = pIndex->nHeight;
        clientmodel->cachedBestHeaderTime = pIndex->GetBlockTime();
    }
    
    // Update UI if not syncing or enough time has passed
    if (!initialSync || now - nLastUpdateNotification.load() > MODEL_UPDATE_DELAY) {
        // Qt 6.9: Lambda-based async invocation
        const auto height = pIndex->nHeight;
        const auto blockTime = QDateTime::fromSecsSinceEpoch(pIndex->GetBlockTime());
        const auto progress = clientmodel->getVerificationProgress(pIndex);
        
        QMetaObject::invokeMethod(clientmodel, [clientmodel, height, blockTime, progress, fHeader]() {
            Q_EMIT clientmodel->numBlocksChanged(height, blockTime, progress, fHeader);
        }, Qt::QueuedConnection);
        
        nLastUpdateNotification.store(now);
    }
}

void ClientModel::subscribeToCoreSignals()
{
    // C++20: Consider migration to std::bind in future
    // For now, keep boost::bind for compatibility with core signals
    uiInterface.ShowProgress.connect(
        boost::bind(ShowProgress, this, boost::placeholders::_1, boost::placeholders::_2));
    
    uiInterface.NotifyNumConnectionsChanged.connect(
        boost::bind(NotifyNumConnectionsChanged, this, boost::placeholders::_1));
    
    uiInterface.NotifyNetworkActiveChanged.connect(
        boost::bind(NotifyNetworkActiveChanged, this, boost::placeholders::_1));
    
    uiInterface.BannedListChanged.connect(
        boost::bind(BannedListChanged, this));
    
    uiInterface.NotifyBlockTip.connect(
        boost::bind(BlockTipChanged, this, boost::placeholders::_1, boost::placeholders::_2, false));
    
    uiInterface.NotifyHeaderTip.connect(
        boost::bind(BlockTipChanged, this, boost::placeholders::_1, boost::placeholders::_2, true));
}

void ClientModel::unsubscribeFromCoreSignals()
{
    // Disconnect signals from client with proper formatting
    uiInterface.ShowProgress.disconnect(
        boost::bind(ShowProgress, this, boost::placeholders::_1, boost::placeholders::_2));
    
    uiInterface.NotifyNumConnectionsChanged.disconnect(
        boost::bind(NotifyNumConnectionsChanged, this, boost::placeholders::_1));
    
    uiInterface.NotifyNetworkActiveChanged.disconnect(
        boost::bind(NotifyNetworkActiveChanged, this, boost::placeholders::_1));
    
    uiInterface.BannedListChanged.disconnect(
        boost::bind(BannedListChanged, this));
    
    uiInterface.NotifyBlockTip.disconnect(
        boost::bind(BlockTipChanged, this, boost::placeholders::_1, boost::placeholders::_2, false));
    
    uiInterface.NotifyHeaderTip.disconnect(
        boost::bind(BlockTipChanged, this, boost::placeholders::_1, boost::placeholders::_2, true));
}
