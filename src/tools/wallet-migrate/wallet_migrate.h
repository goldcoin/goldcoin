#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <memory>
#include <optional>

namespace fs = std::filesystem;

// Migration result codes
enum class MigrationResult {
    SUCCESS = 0,
    BACKUP_FAILED,
    INVALID_SOURCE_WALLET,
    ALREADY_MIGRATED,
    MIGRATION_FAILED,
    VERIFICATION_FAILED,
    USER_CANCELLED,
    UNKNOWN_ERROR
};

// Wallet version detection
enum class WalletDBVersion {
    UNKNOWN = 0,
    BDB_4_8 = 48,
    BDB_18_1 = 181
};

// Migration statistics
struct MigrationStats {
    size_t total_records = 0;
    size_t migrated_records = 0;
    size_t keys_count = 0;
    size_t transactions_count = 0;
    size_t metadata_count = 0;
    std::chrono::milliseconds duration{0};
    size_t original_size = 0;
    size_t new_size = 0;
};

// Main migration class
class WalletMigrator {
public:
    WalletMigrator();
    ~WalletMigrator();

    // Configuration
    void setVerbose(bool verbose) { m_verbose = verbose; }
    void setDryRun(bool dryRun) { m_dryRun = dryRun; }
    void setBackupDir(const fs::path& dir) { m_backupDir = dir; }

    // Core operations
    WalletDBVersion detectVersion(const fs::path& walletPath);
    MigrationResult migrate(const fs::path& walletPath);
    bool verify(const fs::path& walletPath);
    
    // Analysis and reporting
    MigrationStats analyze(const fs::path& walletPath);
    void printReport(const MigrationStats& stats);
    
    // Get last error for detailed information
    std::string getLastError() const { return m_lastError; }

private:
    // Internal operations
    bool createBackup(const fs::path& source);
    bool performMigration(const fs::path& source, const fs::path& dest);
    bool verifyMigration(const fs::path& source, const fs::path& dest);
    bool atomicReplace(const fs::path& source, const fs::path& dest);
    
    // BDB operations
    bool openBDB4(const fs::path& path);
    bool openBDB18(const fs::path& path);
    bool copyRecord(const std::string& key, const std::vector<uint8_t>& value);
    
    // Logging
    void log(const std::string& message);
    void logError(const std::string& error);
    void logProgress(size_t current, size_t total);

    // Member variables
    bool m_verbose = false;
    bool m_dryRun = false;
    fs::path m_backupDir;
    std::string m_lastError;
    
    // Database handles (implementation hidden)
    class Impl;
    std::unique_ptr<Impl> m_impl;
};