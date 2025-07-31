# Understanding Goldcoin's Checkpoint System

## What are Checkpoints?

Checkpoints are a security feature that protects the Goldcoin network from attacks when the mining power (hashrate) is relatively low. Think of them as "save points" that prevent attackers from rewriting blockchain history.

## How Do Checkpoints Protect You?

When Goldcoin's network hashrate is below a certain threshold, the development team can issue checkpoints that:
- Lock in specific blocks as permanent
- Prevent attackers from reversing transactions
- Ensure everyone agrees on the same blockchain history

## Automatic Deactivation

The unique feature of Goldcoin's checkpoint system is that it automatically turns off as the network becomes stronger:

- The system monitors the network's mining power
- When mining power exceeds a growing threshold, checkpoints stop being enforced
- This allows Goldcoin to transition from assisted security to pure decentralized consensus

## Current Status

You can check if checkpoints are currently active by:

1. Checking the network hashrate:
   ```
   goldcoin-cli getnetworkhashps 120
   ```

2. Looking for checkpoint messages in your debug log

## Why This Matters

- **Early Protection**: When Goldcoin's network is young or has low hashrate, checkpoints prevent attacks
- **Future Independence**: As the network grows, it becomes self-sufficient without needing checkpoints
- **Smooth Transition**: The automatic system ensures no sudden changes in security

## For Merchants and Exchanges

- While checkpoints are active, confirmed transactions are extra secure
- After checkpoints deactivate, standard confirmation counts apply
- The transition is designed to happen when the network is already very secure

## Technical Details

For more technical information, see the [full checkpoint documentation](../checkpointing.md).