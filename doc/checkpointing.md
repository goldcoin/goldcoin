# Goldcoin Advanced Checkpointing System (ACP)

## Overview

Goldcoin implements an Advanced Checkpointing System (ACP) that provides protection against 51% attacks during periods of low network hashrate while automatically transitioning to pure proof-of-work consensus as the network grows stronger.

## Purpose

The ACP serves as a security mechanism that:
- Protects the network from reorganization attacks when hashrate is low
- Automatically disengages as the network becomes more secure
- Provides a smooth transition from centralized to decentralized security

## How It Works

### Checkpoint Validation

When a new block is received, the system checks if synchronized checkpoints should be enforced:

1. If the block height is below `syncCheckpointHeight` (configured in chainparams), checkpoints are always enforced
2. If the block height is above `syncCheckpointHeight`, the system calculates the current network hashrate
3. If the network hashrate exceeds the dynamic threshold, checkpoints are ignored and pure PoW consensus takes over

### Dynamic Hashrate Threshold

The threshold is calculated using an exponential growth formula:

```
threshold = 1e12 * 2^(2 * (current_time - 1536541807) / (365 * 24 * 60 * 60))
```

Where:
- `1e12` (1 TH/s) is the base hashrate threshold
- `1536541807` is the Unix timestamp for September 9, 2018 (system activation date)
- The exponent causes the threshold to double every 6 months (182.5 days)

### Example Threshold Values

| Date | Time Since Start | Threshold |
|------|------------------|-----------|
| Sep 2018 | 0 months | 1.0 TH/s |
| Mar 2019 | 6 months | 2.0 TH/s |
| Sep 2019 | 12 months | 4.0 TH/s |
| Sep 2020 | 24 months | 16.0 TH/s |
| Sep 2021 | 36 months | 64.0 TH/s |
| Sep 2022 | 48 months | 256.0 TH/s |
| Sep 2023 | 60 months | 1.0 PH/s |
| Sep 2024 | 72 months | 4.1 PH/s |
| Sep 2025 | 84 months | 16.4 PH/s |
| Sep 2026 | 96 months | 65.5 PH/s |

### Rationale for Growth Rate

The 6-month doubling period was chosen to:
1. Stay ahead of Moore's Law (transistor density doubles approximately every 18-24 months)
2. Account for ASIC development and deployment cycles
3. Provide a buffer for natural network growth
4. Ensure the transition happens well before any single entity could realistically control majority hashrate

## Technical Implementation

### Key Components

1. **Checkpoint Messages**: Signed messages containing block hashes that nodes must follow
2. **Checkpoint Verification**: Ensures checkpoint signatures are valid
3. **Hashrate Calculation**: Uses the last 120 blocks to estimate network hashrate
4. **Threshold Comparison**: Compares current hashrate against the dynamic threshold

### Code Locations

- Main logic: `src/checkpointsync.cpp` - `CheckSyncCheckpoint()`
- Hashrate calculation: `src/rpc/mining.cpp` - `GetNetworkHashPS()`
- Configuration: `src/chainparams.cpp` - `syncCheckpointHeight`

### Configuration Parameters

- `syncCheckpointHeight`: Block height below which checkpoints are always enforced
- Checkpoint public key: Hardcoded in the source for security

## Security Considerations

### Benefits

1. **51% Attack Protection**: Prevents blockchain reorganizations during vulnerable periods
2. **Gradual Decentralization**: Automatically reduces reliance on checkpoints as the network grows
3. **Predictable Transition**: Clear mathematical model for when checkpoints will cease

### Limitations

1. **Centralization Risk**: Checkpoint signing requires a trusted party
2. **Time Dependency**: System relies on accurate time synchronization
3. **One-Way Transition**: Once hashrate exceeds threshold, checkpoints cannot be re-enabled

### Checkpoint Authority

The checkpoint signing key is controlled by the Goldcoin development team. This provides:
- Quick response to attack attempts
- Coordination with exchanges and services during emergencies
- A clear transition plan as the network matures

## Monitoring and Verification

### For Users

Users can monitor the checkpoint system status using RPC commands:

```bash
# Check if a specific block matches the checkpoint
goldcoin-cli getcheckpoint

# Get current network hashrate (120 block average)
goldcoin-cli getnetworkhashps 120

# View sync checkpoint status in debug.log
grep "checkpoint" ~/.goldcoin/debug.log
```

### For Developers

To calculate when checkpoints will cease being enforced:

```python
import time
import math

def calculate_threshold(timestamp):
    base_hashrate = 1e12  # 1 TH/s
    start_time = 1536541807  # Sep 9, 2018
    seconds_per_year = 365 * 24 * 60 * 60
    
    time_elapsed = timestamp - start_time
    years_elapsed = time_elapsed / seconds_per_year
    doublings = 2 * years_elapsed  # Doubles every 6 months
    
    threshold = base_hashrate * math.pow(2, doublings)
    return threshold

# Example: Check threshold for current time
current_threshold = calculate_threshold(time.time())
print(f"Current threshold: {current_threshold / 1e15:.2f} PH/s")
```

## Future Considerations

As the network continues to grow, the checkpoint system will eventually become inactive. At that point:

1. The code can remain as a historical artifact with no performance impact
2. A future hard fork could remove the checkpoint code entirely
3. The transition serves as a milestone in Goldcoin's journey to full decentralization

## FAQ

**Q: When will checkpoints stop being enforced?**
A: When the network hashrate exceeds the calculated threshold. As of 2025, this is approximately 14-16 PH/s.

**Q: Can checkpoints be re-enabled after they're disabled?**
A: No, once the hashrate threshold is exceeded, the network operates purely on proof-of-work consensus.

**Q: What happens if the network hashrate drops below the threshold after exceeding it?**
A: Checkpoints remain disabled. The transition is one-way to prevent manipulation.

**Q: How often are checkpoints issued?**
A: Checkpoints are issued as needed when the system is active, typically for important blocks to prevent reorganization.

## References

- Original implementation: [PPCoin](https://github.com/ppcoin/ppcoin) (Sunny King)
- Adapted from: [Feathercoin](https://github.com/FeatherCoin/Feathercoin) checkpoint implementation
- Related discussion: [Goldcoin PR #60](https://github.com/goldcoin/goldcoin/pull/60) (Advanced Checkpointing)