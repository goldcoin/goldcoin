// Copyright (c) 2007-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Copyright (c) 2011-2017 The Litecoin Core developers
// Copyright (c) 2013-2018 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#ifndef BITCOIN_ADDRDB_H
#define BITCOIN_ADDRDB_H

#include "serialize.h"

#include <string>
#include <map>
#include <boost/filesystem/path.hpp>

class CSubNet;
class CAddrMan;
class CDataStream;

enum class BanReason : int
{
    Unknown          = 0,
    NodeMisbehaving  = 1,
    ManuallyAdded    = 2
};

class CBanEntry
{
public:
    static constexpr int CURRENT_VERSION = 1;
    int nVersion;
    int64_t nCreateTime;
    int64_t nBanUntil;
    uint8_t banReason;

    CBanEntry()
    {
        SetNull();
    }

    CBanEntry(int64_t nCreateTimeIn)
    {
        SetNull();
        nCreateTime = nCreateTimeIn;
    }

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(this->nVersion);
        READWRITE(nCreateTime);
        READWRITE(nBanUntil);
        READWRITE(banReason);
    }

    void SetNull()
    {
        nVersion = CBanEntry::CURRENT_VERSION;
        nCreateTime = 0;
        nBanUntil = 0;
        banReason = static_cast<uint8_t>(BanReason::Unknown);
    }

    std::string banReasonToString()
    {
        switch (static_cast<BanReason>(banReason)) {
        case BanReason::NodeMisbehaving:
            return "node misbehaving";
        case BanReason::ManuallyAdded:
            return "manually added";
        default:
            return "unknown";
        }
    }
};

using banmap_t = std::map<CSubNet, CBanEntry>;

/** Access to the (IP) address database (peers.dat) */
class CAddrDB
{
private:
    boost::filesystem::path pathAddr;
public:
    CAddrDB();
    bool Write(const CAddrMan& addr);
    bool Read(CAddrMan& addr);
    bool Read(CAddrMan& addr, CDataStream& ssPeers);
};

/** Access to the banlist database (banlist.dat) */
class CBanDB
{
private:
    boost::filesystem::path pathBanlist;
public:
    CBanDB();
    bool Write(const banmap_t& banSet);
    bool Read(banmap_t& banSet);
};

#endif // BITCOIN_ADDRDB_H
