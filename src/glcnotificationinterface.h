#ifndef GLCNOTIFICATIONINTERFACE_H
#define GLCNOTIFICATIONINTERFACE_H
// Copyright (c) 2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include "validationinterface.h"

class CGLCNotificationInterface : public CValidationInterface
{
public:
    CGLCNotificationInterface(CConnman& connmanIn): connman(connmanIn) {}
    virtual ~CGLCNotificationInterface() = default;

    // a small helper to initialize current block height in sub-modules on startup
    void InitializeCurrentBlockTip();

protected:
    // CValidationInterface
    //void AcceptedBlockHeader(const CBlockIndex *pindexNew) override;
    //void NotifyHeaderTip(const CBlockIndex *pindexNew, bool fInitialDownload) override;
    void UpdatedBlockTip(const CBlockIndex *pindexNew, const CBlockIndex *pindexFork, bool fInitialDownload) override;
    void SyncTransaction(const CTransaction &tx, const CBlockIndex *pindex, int posInBlock) override;

private:
    CConnman& connman;
};

#endif // GLCNOTIFICATIONINTERFACE_H
