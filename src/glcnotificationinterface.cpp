// Copyright (c) 2014-2019 The Dash Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "glcnotificationinterface.h"
#include "validation.h"

void CGLCNotificationInterface::InitializeCurrentBlockTip()
{
    LOCK(cs_main);
    UpdatedBlockTip(chainActive.Tip(), NULL, IsInitialBlockDownload());
}

void CGLCNotificationInterface::UpdatedBlockTip(const CBlockIndex *pindexNew, const CBlockIndex *pindexFork, bool fInitialDownload)
{
    if (pindexNew == pindexFork) // blocks were disconnected without any new ones
        return;

    // Update global GIP1 activation status
    if(Params().GetConsensus().GIP1Height != -1)
        fGIP1ActiveAtTip = pindexNew->nHeight >= Params().GetConsensus().GIP1Height;
    else
        fGIP1ActiveAtTip = pindexNew->GetMedianTimePast() >= Params().GetConsensus().GIP1ActivationTime;

    if (fInitialDownload)
        return;

}

void CGLCNotificationInterface::SyncTransaction(const CTransaction &tx, const CBlockIndex *pindex, int posInBlock)
{

}

