# GoldCoin Golden River Difficulty Algorithm

**Version 17**  
**September 8, 2025**  
**GoldCoin Core Development Team**

---

## Algorithm Overview

The Golden River difficulty algorithm is GoldCoin's advanced per-block difficulty adjustment system activated after block 251230 (mainnet). It replaces interval-based retargeting with continuous adjustment using statistical analysis of recent block solve times.

## Activation Conditions

```cpp
if (nHeight > params.julyFork2) {
    return GoldenRiver(pindexLast, params);
}
```

**Block Heights:**
- Mainnet: 251230
- Testnet: 2016
- Regtest: 100000000

## Target Parameters

```cpp
const int64_t nTargetTimespanCurrent = 2 * 60 * 60; // Two hours
const int64_t nTargetSpacingCurrent  = 2 * 60;     // Two minutes
```

**Block Time Target:** 2 minutes (120 seconds)  
**Retarget Window:** 2 hours (60 blocks)

## Statistical Analysis Framework

### Sample Collection

The algorithm collects timing data from the last 240 blocks:

```cpp
std::vector<int64_t> last60BlockTimes;
std::vector<int64_t> last120BlockTimes;
```

**Data Points:**
- Last 60 block timestamps (for median calculation)
- Last 120 block timestamps (for average calculation)
- Difficulty values from 60 and 240 blocks ago

### Time Difference Calculation

```cpp
while (xy < 119) {
    if (xy < 59)
        last59TimeDifferences.push_back(llabs(last60BlockTimes[xy] - last60BlockTimes[xy + 1]));
    
    last119TimeDifferences.push_back(llabs(last120BlockTimes[xy] - last120BlockTimes[xy + 1]));
    total += last119TimeDifferences[xy];
}
```

**Calculations:**
- 59 time differences from last 60 blocks
- 119 time differences from last 120 blocks
- Median of 59 differences (position 29 after sorting)
- Average of 119 differences

### Core Metrics

```cpp
sort(last59TimeDifferences.begin(), last59TimeDifferences.end(), comp64);
int64_t nActualTimespan = llabs((last59TimeDifferences[29]));
int64_t medTime = nActualTimespan;
averageTime = total / 119;
medTime = (medTime > averageTime) ? averageTime : medTime;
```

**Primary Metrics:**
- **medTime**: Median solve time from last 59 block intervals
- **averageTime**: Mean solve time from last 119 block intervals
- **Final medTime**: Lesser of median and average

## Emergency Adjustment Conditions

### Massive Difficulty Fall Authorization

```cpp
if (averageTime >= 180 && last119TimeDifferences[0] >= 1200 && last119TimeDifferences[1] >= 1200) {
    didHalfAdjust = true;
    medTime = 240;
}
```

**Conditions:**
- Average time ≥ 180 seconds (3 minutes)
- Two most recent intervals ≥ 1200 seconds (20 minutes)

**Effect:** Authorizes dramatic difficulty reduction

### Deadlock Detection

```cpp
if (medTime >= 120) {
    int numTooClose = 0;
    while (index != 55) {
        if (llabs(last60BlockTimes.at(last60BlockTimes.size() - index) - 
                  last60BlockTimes.at(last60BlockTimes.size() - (index + 5))) == 600) {
            ++numTooClose;
        }
    }
    if (numTooClose > 0) {
        medTime = 119;
    }
}
```

**Detection Logic:**
- Scans for blocks solved in exactly 10-minute intervals
- Indicates 51% defense system interference
- Forces difficulty increase to break deadlock

## Adjustment Rate Limiting

### Standard Adjustment Bounds

```cpp
if (averageTime > 216 || medTime > 122) {
    if (didHalfAdjust) {
        medTime = (int64_t)(120 * 142.0 / 100.0); // 170.4 seconds
    } else {
        medTime = 121; // 120 * 120/119
    }
}
else if (averageTime < 117 || medTime < 117) {
    medTime = 117; // 120 * 98/100
}
```

**Rate Limits:**
- Normal decrease: Maximum 1/119 per block (0.84%)
- Emergency decrease: 42% when authorized
- Increase limit: 2% maximum per block

### Historical Difficulty Constraints

```cpp
// 20% maximum decrease from previous block
bnLast *= 10;
bnLast /= 8;
if (!didHalfAdjust && bnNew > bnLast)
    bnNew.SetCompact(bnLast.GetCompact());

// 2% maximum increase over 60 blocks
bn60ago *= 100;
bn60ago /= 102;
if (bnNew < bn60ago)
    bnNew.SetCompact(bn60ago.GetCompact());

// 8% maximum increase over 240 blocks  
bn240ago *= 100;
bn240ago /= 408;
if (bnNew < bn240ago)
    bnNew.SetCompact(bn240ago.GetCompact());
```

**Multi-timeframe Protection:**
- Single block: 20% maximum decrease
- 60 blocks: 2% cumulative increase limit
- 240 blocks: 8% cumulative increase limit

## Difficulty Calculation

```cpp
nActualTimespan = medTime * 60;

bnNew.SetCompact(pindexLast->nBits);
bnNew *= nActualTimespan;
bnNew /= nTargetTimespanCurrent;
```

**Formula:**
```
newDifficulty = previousDifficulty * (adjustedTime * 60) / (2 * 60 * 60)
newDifficulty = previousDifficulty * adjustedTime / 120
```

## Algorithm Properties

**Responsiveness:**
- Per-block adjustment enables rapid response to hashrate changes
- Statistical analysis reduces noise from individual block variance

**Stability:**
- Multi-timeframe constraints prevent oscillation
- Median filtering reduces impact of outlier solve times

**Attack Resistance:**
- Deadlock detection counters 51% defense system interference  
- Historical constraints limit manipulation effectiveness
- Emergency conditions prevent sustained network disruption

**Computational Efficiency:**
- O(1) median calculation using fixed-size arrays
- Simple arithmetic operations for constraint checking
- Minimal memory footprint with 240-block history

## Integration with 51% Defense

The Golden River algorithm incorporates deadlock detection specifically designed to work with GoldCoin's 51% defense system. When the defense system's rate limiting creates artificial solve time patterns, the algorithm detects these conditions and adjusts accordingly to maintain network flow.