# Building the Wallet Migration Tool

## Prerequisites

### Berkeley DB 18.1
```bash
# Download and build BDB 18.1
wget https://download.oracle.com/berkeley-db/db-18.1.40.tar.gz
tar -xzf db-18.1.40.tar.gz
cd db-18.1.40/build_unix
../dist/configure --enable-cxx --disable-shared --with-pic
make
sudo make install
```

### Development Headers
```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake

# The tool will link against:
# - BDB 18.1 (for new format)
# - Custom compat layer (for reading BDB 4.8)
```

## Build Instructions

### Standalone Build
```bash
cd src/tools/wallet-migrate
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### With Enterprise Build System
```bash
# Add to build-manager.sh:
./build-manager.sh build-tools
```

## Testing Strategy

### 1. Create Test Wallets
```bash
# Use old goldcoind with BDB 4.8 to create test wallets
./goldcoind-old -regtest -daemon
./goldcoin-cli-old createwallet test1
./goldcoin-cli-old generatetoaddress 100 <address>
./goldcoin-cli-old stop
```

### 2. Test Migration
```bash
# Copy test wallet
cp ~/.goldcoin/regtest/wallets/test1/wallet.dat test_wallet.dat

# Test dry run
./wallet-migrate --dry-run --verbose test_wallet.dat

# Test actual migration
./wallet-migrate --migrate --verbose test_wallet.dat

# Verify
./wallet-migrate --verify test_wallet.dat
```

### 3. Test Edge Cases
- Empty wallet
- Encrypted wallet
- Large wallet (>10,000 transactions)
- Corrupted wallet
- Already migrated wallet

## Integration Path

Once the tool is perfected:

1. **Extract Core Logic**
   - Create `src/wallet/migration/` directory
   - Move migration logic to library

2. **Integrate with Daemon**
   ```cpp
   // In src/wallet/wallet.cpp
   if (WalletMigrator::needsMigration(walletPath)) {
       if (GetBoolArg("-automigrate", true)) {
           WalletMigrator::migrate(walletPath);
       }
   }
   ```

3. **Add Configuration**
   ```
   # goldcoin.conf
   automigrate=1  # Automatically migrate old wallets
   keepbackups=1  # Keep wallet backups after migration
   ```

## Safety Checklist

- [ ] Never modify original without backup
- [ ] Verify backup before proceeding
- [ ] Test rollback mechanism
- [ ] Verify all keys preserved
- [ ] Check transaction integrity
- [ ] Test with encrypted wallets
- [ ] Performance test with large wallets
- [ ] Document user recovery process

## Known Issues

1. **BDB 4.8 Compatibility**
   - Need to implement compatibility layer
   - Alternative: Use db_dump/db_load tools

2. **Encryption Handling**
   - Must preserve master key
   - Cannot decrypt during migration

3. **File Locking**
   - Ensure wallet not in use
   - Implement lock detection

## Development Tips

- Use `--verbose` for debugging
- Test on copies, never originals
- Keep detailed logs of migration
- Implement progress callbacks
- Add checksum verification

---

*This tool is critical for user asset protection. Test thoroughly!*