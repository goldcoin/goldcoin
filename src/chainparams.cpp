// Copyright (c) 2007-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Copyright (c) 2011-2017 The Litecoin Core developers
// Copyright (c) 2013-2018 The GoldCoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "The Times 14/May/2013 Justice Dept. Opens Criminal Inquiry Into I.R.S. Audits";
    const CScript genesisOutputScript = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";

        consensus.useNewDifficultyProtocol = true;
        consensus.julyFork = 45000;
        consensus.octoberFork = 100000;
        consensus.novemberFork = 103000;
        consensus.novemberFork2 = 118800;
        consensus.mayFork = 248000;
        consensus.julyFork2 = 251230;
        consensus.febFork = 372000;

        consensus.nEnforceBlockUpgradeMajority = 1500;
        consensus.nRejectBlockOutdatedMajority = 1900;
        consensus.nToCheckBlockUpgradeMajority = 2000;
        consensus.BIP34Height = 10000000;
        consensus.BIP65Height = 10000000;
        consensus.BIP66Height = 10000000;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2.0 * 60 * 60; // Difficulty changes every 60 blocks
        consensus.nPowTargetSpacing = 2.0 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 6048; // 75% of 8064
        consensus.nMinerConfirmationWindow = 8064; // nPowTargetTimespan / nPowTargetSpacing * 4
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1514772000; // January 1st, 2018 - 2AM GMT
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1546308000; // January 1st, 2019 - 2AM GMT

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfd;
        pchMessageStart[1] = 0xc2;
        pchMessageStart[2] = 0xb4;
        pchMessageStart[3] = 0xdd;
        nDefaultPort = 8121;
        nPruneAfterHeight = 100000;

//static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)

        genesis = CreateGenesisBlock(1368560876, 3591624, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xdced3542896ed537cb06f9cb064319adb0da615f64dd8c5e5bad974398f44b24"));
        assert(genesis.hashMerkleRoot == uint256S("0xa215e67ba165202f75b6458d22fedd1a3ec4f03449a4c6b2a4b8130bfebd3b15"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.push_back(CDNSSeedData("seed.gldcoin.com", "seed.gldcoin.com", true));
        vSeeds.push_back(CDNSSeedData("vps.gldcoin.com", "vps.gldcoin.com", true));
        vSeeds.push_back(CDNSSeedData("dnsseed.gldcoin.com", "dnsseed.gldcoin.com", true));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,32);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,50);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,32+128);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
    (     0, uint256S("0xdced3542896ed537cb06f9cb064319adb0da615f64dd8c5e5bad974398f44b24"))
    (     1, uint256S("0xe39be079a4e57af79f63edb2726bdcb401ae520fa5b5328bbeab185b5b3d636e"))
    (     50, uint256S("0x34b021706ae2b72e41d411a31ead78219087213a29fe338515926f055a4c5655"))
    (     150, uint256S("0xc3565831a172cf737b0dd8bd47a81f2dfe99012c97362f397838e617d26ba99b"))
    (     300, uint256S("0x52d9e6919828769b358db66f8793a06e5915948d0a0ee36f59465e91dabb97a6"))
    (     500, uint256S("0x6ba9043e14339790c56181da3ef4d87ff58b8cd3c07b2d33e7d808e189136002"))
    (     1000, uint256S("0x29607502895cf180b7b556f0bc70b2001eacb819be6b4be1e5f0092622e03cb3"))
    (     6048, uint256S("0xe8eb14ac03f25fd52a46b51acba8af543f6b4baf1b85b62d6e6e09ba5f108ed7"))
    (     12096, uint256S("0xd2bd87d677cced55584634d6d03434e041d5dbbaf5639c873a5f6a654d788ad1"))
    (     16128, uint256S("0x290d05c1f5fb2cc75d5bc7657141e4e42d4ee7b4e78a73c96fac878a58d52057"))
    (     18143, uint256S("0x996caca04cce6ffc7f1053abcd350742e2ecc3220663cfd3e3585e3442d7cf74"))
    (     18144, uint256S("0xed5124e191b92d5405374dc4018203a7e03f4251293b6d5f57a83d1e1ff4df30"))
    (     20160, uint256S("0xe19b119f4a633d89320d502e7c05b88d083acdff3b4bd40efcdca54b25f6cb2c"))
    (     20500, uint256S("0x23ab64ad71d7191c28010c7c1b0b35d32ac97ace893dbb20068a6abb617f80a4"))
    (     24000, uint256S("0xc3cf2892cbaaf8b88565f027460bda831a428bf6ea76fafa870bf3586dd07c5f"))
    (     27000, uint256S("0xf7391f58e29d057f152b9c124af6153dadb62385d8728118e9cef728d9a4d16d"))
    (     28000, uint256S("0x28adf712f2a7d9d7ab3836249c9e2beff8d0deb362a1991c61cb61c0fe9af10b"))
    (     29000, uint256S("0x0aca7e1d7cebe224479db62d9887bba7e8dbf5cc295261b6b2e9b9bc76f58ab1"))
    (     30000, uint256S("0x1ff80eac17ba7efc350d65d842cbedd5822b4bef3eae7b1c24424c0d5cc2af51"))
    (     31000, uint256S("0xe9a24595526e9c67357e3a5962e8d489a867573eef1ea104de6be113d26512de"))
    (     35000, uint256S("0x0d8f14bc84ed93490b8c2070de4b744085a4d661f7ef96c856fd32572bbd87fc"))
    (     45000, uint256S("0x612461aaa0ca6a3bc07238ac86e67fa37ae1b8b083d0c1e23f396bbe5bd05896"))
    (     50000, uint256S("0xe251895683ec1363344504b91d9899c29064afc786905c9a052d61ee88a95206"))
    (     60000, uint256S("0xf03feaab75843a39be8cf0fbf8bdae3056aebd4817b89a99e4837db2bdd2659a"))
    (     65000, uint256S("0xb635ce68527e8b777f68a71fe441faab285fa7aafd78259ddc24843539bba369"))
    (     66000, uint256S("0xf619fc8b01c1aedcf4623cea7d85310db85174e27e1b3069dadf76e9bc2f6c99"))
    (     82900, uint256S("0xd411f2115353a132b425c498be3e521b6dbabc424259ce9cba822da46cc41ba4"))
    (     86000, uint256S("0x5a4fac33aa961451860b15dd8a8adfe5c6a0e0ce3fee503ff5b9e5162f2a7ccf"))
    (     100000, uint256S("0x292b50277877a5d7780614f460f669467d09f7d47a84765fb9633c9c78aa035a"))
    (     100989, uint256S("0xb0a9b5749a77c6b3f17935fe220c8a76d56003d2f9e2cbd281f5a7315eae5924"))
    (     115300, uint256S("0x77b3f1913755a4d381f36cd134f280a6519dd54d1c33c499eeded256f36bb394"))
    (     246100, uint256S("0xca7ed43264e0e8965d4115d2f8d3fabb63dcb430fe502d31796c63658ab8274d"))
    (     300000, uint256S("0xe81d2c84c9e7332b35788c1166a0b2c9a34be4d17f08f44a9ba2f5edd82dc300"))
    (     372000, uint256S("0xe3d2857896d0f52ac502eb056ac23f416cb7eddb0a6eba68785b940cca8257ee"))
    (     564000, uint256S("0x9d67ce445d6b513074ef061066bb331871901b953b3bdeaa4dc0a4043cf189f8"))
    };

        chainTxData = ChainTxData{
            // Data as of block b44bc5ae41d1be67227ba9ad875d7268aa86c965b1d64b47c35be6e8d5c352f4 (height 1155626).
            1487715936, // * UNIX timestamp of last known number of transactions
            9243806,  // * total number of transactions between genesis and that timestamp
                    //   (the tx=... number in the SetBestChain debug.log lines)
            0.06     // * estimated number of transactions per second after that timestamp
        };
    }
};
static CMainParams mainParams;
#include "arith_uint256.h"
/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";

        consensus.useNewDifficultyProtocol = true;
        consensus.julyFork = 2016;
        consensus.octoberFork = 2016;
        consensus.novemberFork = 2016;
        consensus.novemberFork2 = 2016;
        consensus.mayFork = 2016;
        consensus.julyFork2 = 2016;
        consensus.febFork = 2016;

        consensus.nEnforceBlockUpgradeMajority = 375;
        consensus.nRejectBlockOutdatedMajority = 475;
        consensus.nToCheckBlockUpgradeMajority = 500;
        consensus.BIP34Height = 1000000; // not activiated
        consensus.BIP65Height = 1000000;
        consensus.BIP66Height = 1000000;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2.0 * 60 * 60; // Difficulty changes every 60 blocks
        consensus.nPowTargetSpacing = 2.0 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1514772000; // January 1st, 2018 - 2AM GMT
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1546308000; // January 1st, 2019 - 2AM GMT

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfc;
        pchMessageStart[1] = 0xc5;
        pchMessageStart[2] = 0xb4;
        pchMessageStart[3] = 0xdd;

        nDefaultPort = 18121;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1513056232, 2702105, 0x1e0ffff0, 1, 50 * COIN);

        consensus.hashGenesisBlock = genesis.GetHash();


        assert(consensus.hashGenesisBlock == uint256S("0x63ee26429068a53a39ab251fb3d611bbac9539651f1bae5d7e73adac3767e513"));
        assert(genesis.hashMerkleRoot == uint256S("0xa215e67ba165202f75b6458d22fedd1a3ec4f03449a4c6b2a4b8130bfebd3b15"));

        vFixedSeeds.clear();
        vSeeds.clear();

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.push_back(CDNSSeedData("testnet.gldcoin.com", "168.235.108.201", true));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x63ee26429068a53a39ab251fb3d611bbac9539651f1bae5d7e73adac3767e513"))
        };


        chainTxData = ChainTxData{
            0,
            0,
            0
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";

        //GoldCoin forks are not activated on RegTest
        consensus.useNewDifficultyProtocol = false;
        consensus.julyFork = 100000000;
        consensus.octoberFork = 100000000;
        consensus.novemberFork = 100000000;
        consensus.novemberFork2 = 100000000;
        consensus.mayFork = 100000000;
        consensus.julyFork2 = 100000000;
        consensus.febFork = 100000001;

        consensus.nEnforceBlockUpgradeMajority =  75000000;
        consensus.nRejectBlockOutdatedMajority =  95000000;
        consensus.nToCheckBlockUpgradeMajority = 100000000;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2.0 * 60 * 60; // Difficulty changes every 60 blocks
        consensus.nPowTargetSpacing = 2.0 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfe;
        pchMessageStart[1] = 0xc3;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xde;
        nDefaultPort = 18130;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1296688602, 4, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x46ca35deafc8f494066bc82f05d7f850479f228ab9196cd659da5fad831122eb"));
        assert(genesis.hashMerkleRoot == uint256S("0xa215e67ba165202f75b6458d22fedd1a3ec4f03449a4c6b2a4b8130bfebd3b15"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x46ca35deafc8f494066bc82f05d7f850479f228ab9196cd659da5fad831122eb"))
        };


        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
