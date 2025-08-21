// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2013-2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "core_io.h"

#include "primitives/block.h"
#include "primitives/transaction.h"
#include "script/script.h"
#include "serialize.h"
#include "streams.h"
#include <univalue.h>
#include "util.h"
#include "utilstrencodings.h"
#include "version.h"

#include <sstream>

CScript ParseScript(const std::string& s)
{
    CScript result;

    static std::map<std::string, opcodetype> mapOpNames;

    if (mapOpNames.empty())
    {
        for (int op = 0; op <= OP_NOP10; op++)
        {
            // Allow OP_RESERVED to get into mapOpNames
            if (op < OP_NOP && op != OP_RESERVED)
                continue;

            const char* name = GetOpName((opcodetype)op);
            if (strcmp(name, "OP_UNKNOWN") == 0)
                continue;
            std::string strName(name);
            mapOpNames[strName] = (opcodetype)op;
            // Convenience: OP_ADD and just ADD are both recognized:
            // Replace first "OP_" with empty string
            size_t pos = strName.find("OP_");
            if (pos != std::string::npos) {
                strName.replace(pos, 3, "");
            }
            mapOpNames[strName] = (opcodetype)op;
        }
    }

    std::vector<std::string> words;
    // Split on whitespace with token compression
    std::stringstream ss(s);
    std::string token;
    while (ss >> token) {
        words.push_back(token);
    }

    for (std::vector<std::string>::const_iterator w = words.begin(); w != words.end(); ++w)
    {
        if (w->empty())
        {
            // Empty string, ignore. (boost::split given '' will return one word)
        }
        else if (w->find_first_not_of("0123456789") == std::string::npos ||
            (w->size() > 1 && (*w)[0] == '-' && w->substr(1).find_first_not_of("0123456789") == std::string::npos))
        {
            // Number
            int64_t n = atoi64(*w);
            result << n;
        }
        else if (w->size() > 2 && w->substr(0, 2) == "0x" && IsHex(w->substr(2)))
        {
            // Raw hex data, inserted NOT pushed onto stack:
            std::vector<unsigned char> raw = ParseHex(std::string(w->begin()+2, w->end()));
            result.insert(result.end(), raw.begin(), raw.end());
        }
        else if (w->size() >= 2 && (*w)[0] == '\'' && (*w)[w->size()-1] == '\'')
        {
            // Single-quoted string, pushed as data. NOTE: this is poor-man's
            // parsing, spaces/tabs/newlines in single-quoted strings won't work.
            std::vector<unsigned char> value(w->begin()+1, w->end()-1);
            result << value;
        }
        else if (mapOpNames.count(*w))
        {
            // opcode, e.g. OP_ADD or ADD:
            result << mapOpNames[*w];
        }
        else
        {
            throw std::runtime_error("script parse error");
        }
    }

    return result;
}

bool DecodeHexTx(CMutableTransaction& tx, const std::string& strHexTx)
{
    if (!IsHex(strHexTx))
        return false;

    std::vector<unsigned char> txData(ParseHex(strHexTx));


    /*DataStream ssData(txData, SER_NETWORK, PROTOCOL_VERSION | SERIALIZE_TRANSACTION_NO_WITNESS);
    try {
        ssData >> tx;
        if (ssData.eof()) {
        return true;
        }
    }
    catch (const std::exception&) {
        // Fall through.
    }*/

    CDataStream ssData(txData, SER_NETWORK, PROTOCOL_VERSION);
    try {
        ssData >> tx;
        if (!ssData.empty())
            return false;
    }
    catch (const std::exception&) {
        return false;
    }

    return true;
}

bool DecodeHexBlk(CBlock& block, const std::string& strHexBlk)
{
    if (!IsHex(strHexBlk))
        return false;

    std::vector<unsigned char> blockData(ParseHex(strHexBlk));
    CDataStream ssBlock(blockData, SER_NETWORK, PROTOCOL_VERSION);
    try {
        ssBlock >> block;
    }
    catch (const std::exception&) {
        return false;
    }

    return true;
}

uint256 ParseHashUV(const UniValue& v, const std::string& strName)
{
    std::string strHex;
    if (v.isStr())
        strHex = v.getValStr();
    return ParseHashStr(strHex, strName);  // Note: ParseHashStr("") throws a runtime_error
}

uint256 ParseHashStr(const std::string& strHex, const std::string& strName)
{
    if (!IsHex(strHex)) // Note: IsHex("") is false
        throw std::runtime_error(strName + " must be hexadecimal string (not '" + strHex + "')");

    uint256 result;
    result.SetHex(strHex);
    return result;
}

std::vector<unsigned char> ParseHexUV(const UniValue& v, const std::string& strName)
{
    std::string strHex;
    if (v.isStr())
        strHex = v.getValStr();
    if (!IsHex(strHex))
        throw std::runtime_error(strName + " must be hexadecimal string (not '" + strHex + "')");
    return ParseHex(strHex);
}
