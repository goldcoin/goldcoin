// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "checkpoints.h"

#include "chain.h"
#include "chainparams.h"
#include "validation.h"
#include "uint256.h"

#include <stdint.h>

#include <boost/foreach.hpp>

namespace Checkpoints {
    

    CBlockIndex* GetLastCheckpoint(const CCheckpointData& data)
    {
        const MapCheckpoints& checkpoints = data.mapCheckpoints;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            BlockMap::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
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

} // namespace Checkpoints
