// Copyright (c) 2007-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Copyright (c) 2011-2017 The Litecoin Core developers
// Copyright (c) 2013-2017 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include "pow.h"

#include "arith_uint256.h"
#include "chain.h"
#include "primitives/block.h"
#include "uint256.h"
#include "util.h"

bool comp64(const int64_t& num1, const int64_t& num2) {
	return num1 > num2;
}

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params) {
	arith_uint256 bnProofOfWorkLimit = UintToArith256(params.powLimit);
	unsigned int nProofOfWorkLimit = bnProofOfWorkLimit.GetCompact();

	// Genesis block
	if (pindexLast == NULL) {
		return nProofOfWorkLimit;
	}

	arith_uint256 bnNew;
	// FeatherCoin difficulty adjustment protocol switch
	static const int nDifficultySwitchHeight = 21000;
	int nHeight = pindexLast->nHeight + 1;
	bool fNewDifficultyProtocol = (nHeight >= nDifficultySwitchHeight /*|| fTestNet*/);

	//julyFork2 whether or not we had a massive difficulty fall authorized
	bool didHalfAdjust = false;

	//moved to solve scope issues
	int64_t averageTime = 120;

	if (nHeight < params.julyFork) {
		//if(!hardForkedJuly) {
		int64_t nTargetTimespan2 = (7 * 24 * 60 * 60) / 8;
		int64_t nTargetSpacing2 = 2.5 * 60;

		int64_t nTargetTimespan2Current = fNewDifficultyProtocol ? nTargetTimespan2 : (nTargetTimespan2 * 4);
		int64_t nInterval = nTargetTimespan2Current / nTargetSpacing2;

		// Only change once per interval, or at protocol switch height
		if ((nHeight % nInterval != 0) &&
		        (nHeight != nDifficultySwitchHeight /*|| fTestNet*/)) {
			// Special difficulty rule for testnet:
			if (params.fPowAllowMinDifficultyBlocks) {
				// If the new block's timestamp is more than 2* 10 minutes
				// then allow mining of a min-difficulty block.
				if (pblock->nTime > pindexLast->nTime + nTargetSpacing2 * 2) {
					return nProofOfWorkLimit;

				} else {
					// Return the last non-special-min-difficulty-rules-block
					const CBlockIndex* pindex = pindexLast;

					while (pindex->pprev && pindex->nHeight % nInterval != 0 && pindex->nBits == nProofOfWorkLimit) {
						pindex = pindex->pprev;
					}

					return pindex->nBits;
				}
			}

			return pindexLast->nBits;
		}

		// GoldCoin (GLD): This fixes an issue where a 51% attack can change difficulty at will.
		// Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
		int blockstogoback = nInterval - 1;

		if ((pindexLast->nHeight + 1) != nInterval) {
			blockstogoback = nInterval;
		}

		const CBlockIndex* pindexFirst = pindexLast;

		for (int i = 0; pindexFirst && i < blockstogoback; i++) {
			pindexFirst = pindexFirst->pprev;
		}

		assert(pindexFirst);

		// Limit adjustment step
		int64_t nActualTimespan = pindexLast->GetBlockTime() - pindexFirst->GetBlockTime();
		LogPrintf("  nActualTimespan = %d  before bounds\n", nActualTimespan);
		int64_t nActualTimespanMax = fNewDifficultyProtocol ? ((nTargetTimespan2Current * 99) / 70) : (nTargetTimespan2Current * 4);
		int64_t nActualTimespanMin = fNewDifficultyProtocol ? ((nTargetTimespan2Current * 70) / 99) : (nTargetTimespan2Current / 4);

		if (nActualTimespan < nActualTimespanMin) {
			nActualTimespan = nActualTimespanMin;
		}

		if (nActualTimespan > nActualTimespanMax) {
			nActualTimespan = nActualTimespanMax;
		}

		// Retarget
		bnNew.SetCompact(pindexLast->nBits);
		bnNew *= nActualTimespan;
		bnNew /= nTargetTimespan2Current;

		if (bnNew > bnProofOfWorkLimit) {
			bnNew = bnProofOfWorkLimit;
		}

		/// debug print
		LogPrintf("GetNextWorkRequired RETARGET\n");
		LogPrintf("nTargetTimespan2 = %d    nActualTimespan = %d\n", nTargetTimespan2Current, nActualTimespan);
		LogPrintf("Before: %08x  %s\n", pindexLast->nBits, arith_uint256().SetCompact(pindexLast->nBits).ToString().c_str());
		LogPrintf("After:  %08x  %s\n", bnNew.GetCompact(), bnNew.ToString().c_str());

	} else
		if (nHeight > params.novemberFork) {
			const_cast<Consensus::Params&>(params).hardForkedNovember = true;

			int64_t nTargetTimespanCurrent = fNewDifficultyProtocol ? params.nPowTargetTimespan : (params.nPowTargetTimespan * 4);
			int64_t nInterval = nTargetTimespanCurrent / params.nPowTargetSpacing;

			// Only change once per interval, or at protocol switch height
			// After julyFork2 we change difficulty at every block.. so we want this only to happen before that..
			if ((nHeight % nInterval != 0) &&
			        (nHeight != nDifficultySwitchHeight /*|| fTestNet*/) && (nHeight <= params.julyFork2)) {
				// Special difficulty rule for testnet:
				if (params.fPowAllowMinDifficultyBlocks) {
					// If the new block's timestamp is more than 2* 10 minutes
					// then allow mining of a min-difficulty block.
					if (pblock->nTime > pindexLast->nTime + params.nPowTargetSpacing * 2) {
						return nProofOfWorkLimit;
					}

					else {
						// Return the last non-special-min-difficulty-rules-block
						const CBlockIndex* pindex = pindexLast;

						while (pindex->pprev && pindex->nHeight % nInterval != 0 && pindex->nBits == nProofOfWorkLimit) {
							pindex = pindex->pprev;
						}

						return pindex->nBits;
					}
				}

				return pindexLast->nBits;
			}

			// GoldCoin (GLD): This fixes an issue where a 51% attack can change difficulty at will.
			// Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
			int blockstogoback = nInterval - 1;

			if ((pindexLast->nHeight + 1) != nInterval) {
				blockstogoback = nInterval;
			}

			const CBlockIndex* pindexFirst = pindexLast;

			for (int i = 0; pindexFirst && i < blockstogoback; i++) {
				pindexFirst = pindexFirst->pprev;
			}

			assert(pindexFirst);

			CBlockIndex tblock1 = *pindexLast;//We want to copy pindexLast to avoid changing it accidentally
			CBlockIndex* tblock2 = &tblock1;

			std::vector<int64_t> last60BlockTimes;

			// Limit adjustment step
			//We need to set this in a way that reflects how fast blocks are actually being solved..
			//First we find the last 60 blocks and take the time between blocks
			//That gives us a list of 59 time differences
			//Then we take the median of those times and multiply it by 60 to get our actualtimespan
			while (last60BlockTimes.size() < 60) {
				last60BlockTimes.push_back(tblock2->GetBlockTime());

				if (tblock2->pprev) { //should always be so
					tblock2 = tblock2->pprev;
				}
			}

			std::vector<int64_t> last59TimeDifferences;

			int xy = 0;

			while (last59TimeDifferences.size() != 59) {
				if (xy == 59) {
					LogPrintf(" GetNextWorkRequired(): This shouldn't have happened \n");
					break;
				}

				last59TimeDifferences.push_back(llabs(last60BlockTimes[xy] - last60BlockTimes[xy + 1]));
				xy++;
			}

			sort(last59TimeDifferences.begin(), last59TimeDifferences.end(), comp64);

			LogPrintf("  Median Time between blocks is: %d \n", last59TimeDifferences[29]);
			int64_t nActualTimespan = llabs((last59TimeDifferences[29]));
			int64_t medTime = nActualTimespan;

			if (nHeight > params.mayFork) {


				//Difficulty Fix here for case where average time between blocks becomes far longer than 2 minutes, even though median time is close to 2 minutes.
				//Uses the last 120 blocks(Should be 4 hours) for calculating

				LogPrintf(" GetNextWorkRequired(): May Fork mode \n");

				CBlockIndex tblock1 = *pindexLast;//We want to copy pindexLast to avoid changing it accidentally
				CBlockIndex* tblock2 = &tblock1;

				std::vector<int64_t> last120BlockTimes;

				// Limit adjustment step
				//We need to set this in a way that reflects how fast blocks are actually being solved..
				//First we find the last 120 blocks and take the time between blocks
				//That gives us a list of 119 time differences
				//Then we take the average of those times and multiply it by 60 to get our actualtimespan
				while (last120BlockTimes.size() < 120) {
					last120BlockTimes.push_back(tblock2->GetBlockTime());

					if (tblock2->pprev) { //should always be so
						tblock2 = tblock2->pprev;
					}
				}

				std::vector<int64_t> last119TimeDifferences;

				int xy = 0;

				while (last119TimeDifferences.size() != 119) {
					if (xy == 119) {
						LogPrintf(" GetNextWorkRequired(): This shouldn't have happened 2 \n");
						break;
					}

					last119TimeDifferences.push_back(llabs(last120BlockTimes[xy] - last120BlockTimes[xy + 1]));
					xy++;
				}

				int64_t total = 0;

				for (int x = 0; x < 119; x++) {
					int64_t timeN = last119TimeDifferences[x];
					//LogPrintf(" GetNextWorkRequired(): Current Time difference is: %"PRI64d" \n",timeN);
					total += timeN;
				}

				averageTime = total / 119;


				LogPrintf(" GetNextWorkRequired(): Average time between blocks over the last 120 blocks is: %d \n", averageTime);
				/*  LogPrintf(" GetNextWorkRequired(): Total Time (over 119 time differences) is: %"PRI64d" \n",total);
				    LogPrintf(" GetNextWorkRequired(): First Time (over 119 time differences) is: %"PRI64d" \n",last119TimeDifferences[0]);
				    LogPrintf(" GetNextWorkRequired(): Last Time (over 119 time differences) is: %"PRI64d" \n",last119TimeDifferences[118]);
				    LogPrintf(" GetNextWorkRequired(): Last Time is: %"PRI64d" \n",last120BlockTimes[119]);
				    LogPrintf(" GetNextWorkRequired(): 2nd Last Time is: %"PRI64d" \n",last120BlockTimes[118]);
				    LogPrintf(" GetNextWorkRequired(): First Time is: %"PRI64d" \n",last120BlockTimes[0]);
				    LogPrintf(" GetNextWorkRequired(): 2nd Time is: %"PRI64d" \n",last120BlockTimes[1]);*/

				if (nHeight <= params.julyFork2) {
					//If the average time between blocks exceeds or is equal to 3 minutes then increase the med time accordingly
					if (averageTime >= 180) {
						LogPrintf(" \n Average Time between blocks is too high.. Attempting to Adjust.. \n ");
						medTime = 130;

					} else
						if (averageTime >= 108 && medTime < 120) {
							//If the average time between blocks is more than 1.8 minutes and medTime is less than 120 seconds (which would ordinarily prompt an increase in difficulty)
							//limit the stepping to something reasonable(so we don't see massive difficulty spike followed by miners leaving in these situations).
							medTime = 110;
							LogPrintf(" \n Medium Time between blocks is too low compared to average time.. Attempting to Adjust.. \n ");
						}

				} else {//julyFork2 changes here

					//Calculate difficulty of previous block as a double
					/*  int nShift = (pindexLast->nBits >> 24) & 0xff;
					    double dDiff =
					    (double)0x0000ffff / (double)(pindexLast->nBits & 0x00ffffff);
					    while (nShift < 29)
					    {
					    dDiff *= 256.0;
					    nShift++;
					    }
					    while (nShift > 29)
					    {
					    dDiff /= 256.0;
					    nShift--;
					    } */

					//int64 hashrate = (int64)(dDiff * pow(2.0,32.0))/((medTime > averageTime)?averageTime:medTime);

					medTime = (medTime > averageTime) ? averageTime : medTime;

					if (averageTime >= 180 && last119TimeDifferences[0] >= 1200 && last119TimeDifferences[1] >= 1200) {
						didHalfAdjust = true;
						medTime = 240;
					}

				}
			}

			//Fixes an issue where median time between blocks is greater than 120 seconds and is not permitted to be lower by the defence system
			//Causing difficulty to drop without end

			if (nHeight > params.novemberFork2) {
				if (medTime >= 120) {
					//Check to see whether we are in a deadlock situation with the 51% defense system
					LogPrintf(" \n Checking for DeadLocks \n");
					int numTooClose = 0;
					int index = 1;

					while (index != 55) {
						if (llabs(last60BlockTimes.at(last60BlockTimes.size() - index) - last60BlockTimes.at(last60BlockTimes.size() - (index + 5))) == 600) {
							numTooClose++;
						}

						index++;
					}

					if (numTooClose > 0) {
						//We found 6 blocks that were solved in exactly 10 minutes
						//Averaging 1.66 minutes per block
						LogPrintf(" \n DeadLock detected and fixed - Difficulty Increased to avoid bleeding edge of defence system \n");

						if (nHeight > params.julyFork2) {
							medTime = 119;

						} else {
							medTime = 110;
						}

					} else {
						LogPrintf(" \n DeadLock not detected. \n");
					}


				}
			}


			if (nHeight > params.julyFork2) {
				//216 == (int64) 180.0/100.0 * 120
				//122 == (int64) 102.0/100.0 * 120 == 122.4
				if (averageTime > 216 || medTime > 122) {
					if (didHalfAdjust) {
						// If the average time between blocks was
						// too high.. allow a dramatic difficulty
						// fall..
						medTime = (int64_t)(120 * 142.0 / 100.0);

					} else {
						// Otherwise only allow a 120/119 fall per block
						// maximum.. As we now adjust per block..
						// 121 == (int64) 120 * 120.0/119.0
						medTime = 121;
					}
				}

				// 117 -- (int64) 120.0 * 98.0/100.0
				else
					if (averageTime < 117 || medTime < 117)  {
						// If the average time between blocks is within 2% of target
						// value
						// Or if the median time stamp between blocks is within 2% of
						// the target value
						// Limit diff increase to 2%
						medTime = 117;
					}

				nActualTimespan = medTime * 60;

			} else {
				nActualTimespan = medTime * 60;

				LogPrintf("  nActualTimespan = %d  before bounds\n", nActualTimespan);
				int64_t nActualTimespanMax = fNewDifficultyProtocol ? ((nTargetTimespanCurrent * 99) / 70) : (nTargetTimespanCurrent * 4);
				int64_t nActualTimespanMin = fNewDifficultyProtocol ? ((nTargetTimespanCurrent * 70) / 99) : (nTargetTimespanCurrent / 4);

				if (nActualTimespan < nActualTimespanMin) {
					nActualTimespan = nActualTimespanMin;
				}

				if (nActualTimespan > nActualTimespanMax) {
					nActualTimespan = nActualTimespanMax;
				}
			}


			if (nHeight > params.julyFork2) {
				CBlockIndex tblock11 = *pindexLast;//We want to copy pindexLast to avoid changing it accidentally
				CBlockIndex* tblock22 = &tblock11;

				// We want to limit the possible difficulty raise/fall over 60 and 240 blocks here
				// So we get the difficulty at 60 and 240 blocks ago

				int64_t nbits60ago = 0ULL;
				int64_t nbits240ago = 0ULL;
				int counter = 0;

				//Note: 0 is the current block, we want 60 past current
				while (counter <= 240) {
					if (counter == 60) {
						nbits60ago = tblock22->nBits;

					} else
						if (counter == 240) {
							nbits240ago = tblock22->nBits;
						}

					if (tblock22->pprev) { //should always be so
						tblock22 = tblock22->pprev;
					}

					counter++;
				}

				//Now we get the old targets
				arith_uint256 bn60ago = 0, bn240ago = 0, bnLast = 0;
				bn60ago.SetCompact(nbits60ago);
				bn240ago.SetCompact(nbits240ago);
				bnLast.SetCompact(pindexLast->nBits);

				//Set the new target
				bnNew.SetCompact(pindexLast->nBits);
				bnNew *= nActualTimespan;
				bnNew /= nTargetTimespanCurrent;


				//Now we have the difficulty at those blocks..

				// Set a floor on difficulty decreases per block(20% lower maximum
				// than the previous block difficulty).. when there was no halfing
				// necessary.. 10/8 == 1.0/0.8
				bnLast *= 10;
				bnLast /= 8;

				if (!didHalfAdjust && bnNew > bnLast) {
					bnNew.SetCompact(bnLast.GetCompact());
				}

				bnLast *= 8;
				bnLast /= 10;

				// Set ceilings on difficulty increases per block

				//1.0/1.02 == 100/102
				bn60ago *= 100;
				bn60ago /= 102;

				if (bnNew < bn60ago) {
					bnNew.SetCompact(bn60ago.GetCompact());
				}

				bn60ago *= 102;
				bn60ago /= 100;

				//1.0/(1.02*4) ==  100 / 408

				bn240ago *= 100;
				bn240ago /= 408;

				if (bnNew < bn240ago) {
					bnNew.SetCompact(bn240ago.GetCompact());
				}

				bn240ago *= 408;
				bn240ago /= 100;

			} else {
				// RetargetBitcoin
				bnNew.SetCompact(pindexLast->nBits);
				bnNew *= nActualTimespan;
				bnNew /= nTargetTimespanCurrent;
			}

			//Sets a ceiling on highest target value (lowest possible difficulty)
			if (bnNew > bnProofOfWorkLimit) {
				bnNew = bnProofOfWorkLimit;
			}

			/// debug print
			LogPrintf("GetNextWorkRequired RETARGET\n");
			LogPrintf("nTargetTimespan = %d    nActualTimespan = %d\n", nTargetTimespanCurrent, nActualTimespan);
			LogPrintf("Before: %08x  %s\n", pindexLast->nBits, arith_uint256().SetCompact(pindexLast->nBits).ToString().c_str());
			LogPrintf("After:  %08x  %s\n", bnNew.GetCompact(), bnNew.ToString().c_str());

		} else {
			const_cast<Consensus::Params&>(params).hardForkedJuly = true;
			int64_t nTargetTimespanCurrent = fNewDifficultyProtocol ? params.nPowTargetTimespan : (params.nPowTargetTimespan * 4);
			int64_t nInterval = nTargetTimespanCurrent / params.nPowTargetSpacing;

			// Only change once per interval, or at protocol switch height
			if ((nHeight % nInterval != 0) &&
			        (nHeight != nDifficultySwitchHeight /*|| fTestNet*/)) {
				// Special difficulty rule for testnet:
				if (params.fPowAllowMinDifficultyBlocks) {
					// If the new block's timestamp is more than 2* 10 minutes
					// then allow mining of a min-difficulty block.
					if (pblock->nTime > pindexLast->nTime + params.nPowTargetSpacing * 2) {
						return nProofOfWorkLimit;
					}

					else {
						// Return the last non-special-min-difficulty-rules-block
						const CBlockIndex* pindex = pindexLast;

						while (pindex->pprev && pindex->nHeight % nInterval != 0 && pindex->nBits == nProofOfWorkLimit) {
							pindex = pindex->pprev;
						}

						return pindex->nBits;
					}
				}

				return pindexLast->nBits;
			}

			// GoldCoin (GLD): This fixes an issue where a 51% attack can change difficulty at will.
			// Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
			int blockstogoback = nInterval - 1;

			if ((pindexLast->nHeight + 1) != nInterval) {
				blockstogoback = nInterval;
			}

			const CBlockIndex* pindexFirst = pindexLast;

			for (int i = 0; pindexFirst && i < blockstogoback; i++) {
				pindexFirst = pindexFirst->pprev;
			}

			assert(pindexFirst);

			// Limit adjustment step
			int64_t nActualTimespan = pindexLast->GetBlockTime() - pindexFirst->GetBlockTime();
			LogPrintf("  nActualTimespan = %d  before bounds\n", nActualTimespan);
			int64_t nActualTimespanMax = fNewDifficultyProtocol ? ((nTargetTimespanCurrent * 99) / 70) : (nTargetTimespanCurrent * 4);
			int64_t nActualTimespanMin = fNewDifficultyProtocol ? ((nTargetTimespanCurrent * 70) / 99) : (nTargetTimespanCurrent / 4);

			if (nActualTimespan < nActualTimespanMin) {
				nActualTimespan = nActualTimespanMin;
			}

			if (nActualTimespan > nActualTimespanMax) {
				nActualTimespan = nActualTimespanMax;
			}

			// Retarget
			bnNew.SetCompact(pindexLast->nBits);
			bnNew *= nActualTimespan;
			bnNew /= nTargetTimespanCurrent;

			if (bnNew > bnProofOfWorkLimit) {
				bnNew = bnProofOfWorkLimit;
			}

			/// debug print
			LogPrintf("GetNextWorkRequired RETARGET\n");
			LogPrintf("nTargetTimespan = %d    nActualTimespan = %d\n", nTargetTimespanCurrent, nActualTimespan);
			LogPrintf("Before: %08x  %s\n", pindexLast->nBits, arith_uint256().SetCompact(pindexLast->nBits).ToString().c_str());
			LogPrintf("After:  %08x  %s\n", bnNew.GetCompact(), bnNew.ToString().c_str());
		}

	return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params) {
	bool fNegative;
	bool fOverflow;
	arith_uint256 bnTarget;

	bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

	// Check range
	if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit)) {
		return false;
	}

	// Check proof of work matches claimed amount
	if (UintToArith256(hash) > bnTarget) {
		return false;
	}

	return true;
}
