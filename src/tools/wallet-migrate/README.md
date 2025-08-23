# Goldcoin Wallet Migration Tool

## Purpose
Standalone tool for migrating Goldcoin wallets from Berkeley DB 4.8 to Berkeley DB 18.1.
This tool is designed for testing and experimentation before integration into the main daemon.

## Design Philosophy
- **Standalone**: Completely independent from goldcoind
- **Safe**: Never modifies original wallet without explicit confirmation
- **Verbose**: Detailed logging for debugging
- **Testable**: Can be run repeatedly without side effects

## Usage
```bash
# Analyze wallet without making changes
./wallet-migrate --analyze wallet.dat

# Perform migration with automatic backup
./wallet-migrate --migrate wallet.dat

# Dry run - simulate migration without changes
./wallet-migrate --dry-run wallet.dat

# Verify migrated wallet
./wallet-migrate --verify wallet.dat
```

## Development Benefits
1. **Rapid Testing**: No need to rebuild entire daemon
2. **Safe Experimentation**: Test on copies without risk
3. **Clear Separation**: Migration logic isolated from wallet code
4. **Easy Debugging**: Standalone tool easier to debug
5. **User Testing**: Can share tool with advanced users for testing

## Build Instructions
```bash
cd src/tools/wallet-migrate
mkdir build && cd build
cmake ..
make
```

## Testing Protocol
1. Create test wallets with various configurations
2. Test migration on copies
3. Verify all data preserved
4. Test error conditions
5. Performance testing with large wallets

## Integration Path
Once perfected, the migration code can be:
1. Wrapped in a library
2. Called from goldcoind at startup
3. Integrated into wallet loading logic

This approach minimizes risk and maximizes testing capability.