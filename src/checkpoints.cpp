// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2013-2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "checkpoints.h"

#include "chain.h"
#include "chainparams.h"
#include "validation.h"
#include "uint256.h"

#include <stdint.h>

// C++23: Range-based for loops replace BOOST_FOREACH

namespace Checkpoints {
    

    CBlockIndex* GetLastCheckpoint(const CCheckpointData& data)
    {
        const MapCheckpoints& checkpoints = data.mapCheckpoints;

        // C++23: Use reverse iterator instead of BOOST_REVERSE_FOREACH
        for (auto it = checkpoints.rbegin(); it != checkpoints.rend(); ++it)
        {
            const uint256& hash = it->second;
            BlockMap::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return nullptr;
    }

    //Memory only!
    void AddCheckPoint(const CCheckpointData& data, int64_t height, uint256 hash)
    {
        MapCheckpoints& checkpoints = const_cast<MapCheckpoints&>(data.mapCheckpoints);
        checkpoints.insert(std::pair<int64_t,uint256>(height, hash));
    }

    //Memory only!
    bool IsBadpoint(const CBadpointData& data, int64_t height, uint256 hash) 
    {
        const MapCheckpoints& badpoints = data.mapBadpoints;
        std::map<int64_t, uint256>::iterator result = const_cast<MapCheckpoints&>(badpoints).find(height);
        if(result != badpoints.end())
            return result->second == hash;
        return false;        
    }

    //Memory only!
    void AddBadPoint(const CBadpointData& data, int64_t height, uint256 hash) 
    {
        const_cast<MapCheckpoints&>(data.mapBadpoints).insert(std::pair<int64_t,uint256>(height, hash));
    }

    uint256 GetLatestHardenedCheckpoint(const CCheckpointData& data)
    {
        const MapCheckpoints& checkpoints = data.mapCheckpoints;

        if (checkpoints.empty())
            return Params().GetConsensus().hashGenesisBlock;

        return (checkpoints.rbegin()->second);
    }

} // namespace Checkpoints
