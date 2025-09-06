// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_WALLET_MIGRATE_H
#define BITCOIN_WALLET_MIGRATE_H

#include <fs.h>
#include <string>

namespace WalletMigration {

enum class WalletDBVersion {
    UNKNOWN,
    BDB_4_8,
    BDB_18_1
};

/**
 * Detect the Berkeley DB version of a wallet file
 * @param walletPath Path to wallet.dat
 * @return Detected database version
 */
WalletDBVersion DetectWalletVersion(const fs::path& walletPath);

/**
 * Check if wallet needs migration from BDB 4.8 to 18.1
 * @param walletPath Path to wallet.dat
 * @return true if migration needed
 */
bool NeedsMigration(const fs::path& walletPath);

/**
 * Silently migrate wallet from BDB 4.8 to BDB 18.1
 * Creates automatic backup before migration
 * @param walletPath Path to wallet.dat
 * @return true if migration successful
 */
bool MigrateWallet(const fs::path& walletPath);

/**
 * Create a timestamped backup of the wallet
 * @param walletPath Source wallet path
 * @return Path to backup file, empty on failure
 */
fs::path CreateMigrationBackup(const fs::path& walletPath);

/**
 * Inject wallet metadata using direct BDB API (the breakthrough function)
 * @param wallet_path Path to migrated wallet
 * @param err Error message output
 * @return true if injection successful
 */
bool InjectWalletMetadata(const std::string& wallet_path, std::string& err);

} // namespace WalletMigration

#endif // BITCOIN_WALLET_MIGRATE_H