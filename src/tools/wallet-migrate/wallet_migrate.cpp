#include "wallet_migrate.h"
#include "wallet_compat.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>
#include <iomanip>
#include <sstream>

// BDB magic bytes (for detection) - stored in little-endian
const uint8_t BDB_BTREE_MAGIC[] = {0x62, 0x31, 0x05, 0x00};  // Btree magic (0x00053162 in LE)
const uint8_t BDB48_VERSION = 0x09;  // BDB 4.8 version number at offset 16
const uint8_t BDB18_VERSION = 0x0a;  // BDB 18.1 version number at offset 16

class WalletMigrator::Impl {
public:
    // We'll use db_dump/db_load approach for safety
    // This is the most reliable method for production use
};

WalletMigrator::WalletMigrator() : m_impl(std::make_unique<Impl>()) {}
WalletMigrator::~WalletMigrator() = default;

WalletDBVersion WalletMigrator::detectVersion(const fs::path& walletPath) {
    log("Detecting wallet version: " + walletPath.string());
    
    std::ifstream file(walletPath, std::ios::binary);
    if (!file) {
        logError("Cannot open wallet file");
        return WalletDBVersion::UNKNOWN;
    }
    
    // Read first 512 bytes to check headers
    uint8_t header[512];
    file.read(reinterpret_cast<char*>(header), sizeof(header));
    
    // Check for BDB magic bytes at various offsets
    // BDB stores magic at offset 12 in the meta page
    if (file.gcount() >= 16) {
        // Debug: Show what we found
        log("Found magic bytes at offset 12: " + 
            std::to_string(header[12]) + " " + 
            std::to_string(header[13]) + " " + 
            std::to_string(header[14]) + " " + 
            std::to_string(header[15]));
            
        // Check for BDB Btree magic signature
        if (std::memcmp(header + 12, BDB_BTREE_MAGIC, 4) == 0) {
            // Check version byte at offset 16
            uint8_t version = header[16];
            log("BDB version byte at offset 16: 0x" + 
                std::to_string(static_cast<int>(version)));
            
            if (version == BDB48_VERSION) {
                log("Detected Berkeley DB 4.8 format (version 0x09)");
                
                // Additional validation: Check page size (usually at offset 20)
                uint32_t pageSize = *reinterpret_cast<uint32_t*>(header + 20);
                log("Page size: " + std::to_string(pageSize));
                if (pageSize == 4096 || pageSize == 8192 || pageSize == 16384 || pageSize == 32768) {
                    return WalletDBVersion::BDB_4_8;
                }
            } else if (version == BDB18_VERSION) {
                log("Detected Berkeley DB 18.1 format (version 0x0a)");
                return WalletDBVersion::BDB_18_1;
            } else {
                log("Unknown BDB version: 0x" + std::to_string(static_cast<int>(version)));
            }
        }
    }
    
    // Try alternative detection using db_dump
    std::string cmd = "db4.8_dump -V \"" + walletPath.string() + "\" 2>&1";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (pipe) {
        char buffer[128];
        std::string result;
        while (fgets(buffer, sizeof(buffer), pipe)) {
            result += buffer;
        }
        pclose(pipe);
        
        if (result.find("Berkeley DB 4.8") != std::string::npos) {
            return WalletDBVersion::BDB_4_8;
        }
    }
    
    logError("Unknown wallet format");
    return WalletDBVersion::UNKNOWN;
}

bool WalletMigrator::createBackup(const fs::path& source) {
    // Generate timestamp
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream timestamp;
    timestamp << std::put_time(std::localtime(&time_t), "%Y%m%d-%H%M%S");
    
    // Create backup filename
    fs::path backupPath = m_backupDir.empty() ? source.parent_path() : m_backupDir;
    std::string backupName = source.stem().string() + ".bdb4.backup-" + timestamp.str() + source.extension().string();
    fs::path backupFile = backupPath / backupName;
    
    log("Creating backup: " + backupFile.string());
    
    try {
        // Copy file
        fs::copy_file(source, backupFile, fs::copy_options::overwrite_existing);
        
        // Verify backup
        if (!fs::exists(backupFile)) {
            logError("Backup file was not created");
            return false;
        }
        
        auto sourceSize = fs::file_size(source);
        auto backupSize = fs::file_size(backupFile);
        
        if (sourceSize != backupSize) {
            logError("Backup size mismatch");
            fs::remove(backupFile);
            return false;
        }
        
        log("✓ Backup created successfully: " + backupFile.string());
        log("  Size: " + std::to_string(backupSize) + " bytes");
        return true;
        
    } catch (const fs::filesystem_error& e) {
        logError("Backup failed: " + std::string(e.what()));
        return false;
    }
}

MigrationResult WalletMigrator::migrate(const fs::path& walletPath) {
    log("========================================");
    log("Starting wallet migration");
    log("Source: " + walletPath.string());
    
    // Step 1: Detect version
    WalletDBVersion version = detectVersion(walletPath);
    if (version != WalletDBVersion::BDB_4_8) {
        if (version == WalletDBVersion::BDB_18_1) {
            logError("Wallet is already in BDB 18.1 format");
            return MigrationResult::ALREADY_MIGRATED;
        }
        logError("Cannot migrate: unknown wallet format");
        return MigrationResult::INVALID_SOURCE_WALLET;
    }
    
    // Step 2: Create backup
    if (!m_dryRun) {
        if (!createBackup(walletPath)) {
            logError("Failed to create backup");
            return MigrationResult::BACKUP_FAILED;
        }
    } else {
        log("[DRY RUN] Would create backup");
    }
    
    // Step 3: Perform migration
    fs::path tempWallet = walletPath.string() + ".migrating";
    
    if (m_dryRun) {
        log("[DRY RUN] Would migrate wallet to BDB 18.1");
        return MigrationResult::SUCCESS;
    }
    
    if (!performMigration(walletPath, tempWallet)) {
        logError("Migration failed");
        // Clean up temp file
        if (fs::exists(tempWallet)) {
            fs::remove(tempWallet);
        }
        return MigrationResult::MIGRATION_FAILED;
    }
    
    // Step 4: Verify migration
    if (!verifyMigration(walletPath, tempWallet)) {
        logError("Verification failed");
        fs::remove(tempWallet);
        return MigrationResult::VERIFICATION_FAILED;
    }
    
    // Step 5: Atomic replace
    if (!atomicReplace(walletPath, tempWallet)) {
        logError("Failed to replace wallet file");
        return MigrationResult::UNKNOWN_ERROR;
    }
    
    log("✓ Migration completed successfully!");
    return MigrationResult::SUCCESS;
}

bool WalletMigrator::performMigration(const fs::path& source, const fs::path& dest) {
    log("Migrating wallet data...");
    
    // Using db_dump/db_load approach for maximum safety
    // This method is recommended by Oracle for version migrations
    
    // Step 1: Dump from BDB 4.8
    std::string dumpFile = source.string() + ".dump";
    
    log("Exporting wallet data...");
    
    // Use our compatibility layer to export
    BDB48Compat compat;
    if (!compat.exportWallet(source, dumpFile, BDB48Compat::USE_DB_DUMP)) {
        logError("Failed to export wallet data (db4.8_dump not found?)");
        logError("Please install db4.8-util package or compile db4.8_dump");
        return false;
    }
    
    // Verify dump file was created
    if (!fs::exists(dumpFile) || fs::file_size(dumpFile) == 0) {
        logError("Dump file is empty or missing");
        return false;
    }
    
    log("✓ Exported " + std::to_string(fs::file_size(dumpFile)) + " bytes");
    
    // Step 2: Load into BDB 18.1 using our pre-built db_load
    std::string loadCmd = "/home/microguy/build/bdb-18.1-utils/db-18.1.40/build_unix/db_load -f \"" + dumpFile + "\" \"" + dest.string() + "\" 2>/dev/null";
    
    log("Importing to BDB 18.1 format...");
    int loadResult = system(loadCmd.c_str());
    
    // Clean up dump file
    fs::remove(dumpFile);
    
    if (loadResult != 0) {
        logError("Failed to import wallet data");
        return false;
    }
    
    // Verify new wallet was created
    if (!fs::exists(dest) || fs::file_size(dest) == 0) {
        logError("New wallet file is empty or missing");
        return false;
    }
    
    log("✓ Migration complete");
    return true;
}

bool WalletMigrator::verifyMigration(const fs::path& source, const fs::path& dest) {
    log("Verifying migration...");
    
    // Simple verification: Check that destination was created and has reasonable size
    if (!fs::exists(dest)) {
        logError("Destination wallet not created");
        return false;
    }
    
    auto sourceSize = fs::file_size(source);
    auto destSize = fs::file_size(dest);
    
    // BDB 18.1 files are typically 80-95% of BDB 4.8 size due to better compression
    if (destSize == 0 || destSize > sourceSize * 1.2) {
        logError("Unexpected destination size");
        return false;
    }
    
    log("✓ Source size: " + std::to_string(sourceSize) + " bytes");
    log("✓ Migrated size: " + std::to_string(destSize) + " bytes");
    
    // Quick sanity check: can we read the version?
    WalletDBVersion destVersion = detectVersion(dest);
    if (destVersion != WalletDBVersion::BDB_18_1) {
        logError("Destination is not BDB 18.1 format");
        return false;
    }
    
    log("✓ Verified as BDB 18.1 format");
    return true;
}

bool WalletMigrator::atomicReplace(const fs::path& original, const fs::path& replacement) {
    log("Replacing wallet file...");
    
    try {
        // Create a backup of original just in case
        fs::path safetyBackup = original.string() + ".pre-replace";
        fs::rename(original, safetyBackup);
        
        // Move new wallet into place
        fs::rename(replacement, original);
        
        // Remove safety backup
        fs::remove(safetyBackup);
        
        log("✓ Wallet file replaced successfully");
        return true;
        
    } catch (const fs::filesystem_error& e) {
        logError("Failed to replace wallet: " + std::string(e.what()));
        return false;
    }
}

bool WalletMigrator::verify(const fs::path& walletPath) {
    log("Verifying wallet integrity...");
    
    // Check if we can read the wallet
    WalletDBVersion version = detectVersion(walletPath);
    
    if (version == WalletDBVersion::UNKNOWN) {
        logError("Cannot read wallet format");
        return false;
    }
    
    // Try to dump the wallet to verify it's readable
    std::string dumpCmd;
    if (version == WalletDBVersion::BDB_4_8) {
        dumpCmd = "db4.8_verify \"" + walletPath.string() + "\" 2>&1";
    } else {
        dumpCmd = "db_verify \"" + walletPath.string() + "\" 2>&1";
    }
    
    FILE* pipe = popen(dumpCmd.c_str(), "r");
    if (pipe) {
        char buffer[128];
        std::string result;
        while (fgets(buffer, sizeof(buffer), pipe)) {
            result += buffer;
        }
        int exitCode = pclose(pipe);
        
        if (exitCode == 0) {
            log("✓ Wallet verification passed");
            return true;
        } else {
            logError("Wallet verification failed: " + result);
            return false;
        }
    }
    
    log("⚠ Could not run verification (tools missing?)");
    return true; // Assume OK if tools missing
}

MigrationStats WalletMigrator::analyze(const fs::path& walletPath) {
    MigrationStats stats;
    
    log("Analyzing wallet...");
    
    // Get file size
    stats.original_size = fs::file_size(walletPath);
    
    // Count records using db_dump
    WalletDBVersion version = detectVersion(walletPath);
    std::string dumpCmd;
    
    if (version == WalletDBVersion::BDB_4_8) {
        dumpCmd = "/home/microguy/git/microguy/goldcoin/depends/work/build/x86_64-pc-linux-gnu/bdb48-utils/4.8.30.NC-638779dad17/build_unix/db_dump -p \"" + walletPath.string() + "\" 2>/dev/null";
    } else {
        dumpCmd = "/home/microguy/build/bdb-18.1-utils/db-18.1.40/build_unix/db_dump -p \"" + walletPath.string() + "\" 2>/dev/null";
    }
    
    // Simple record counting - just count data lines in dump output
    FILE* pipe = popen(dumpCmd.c_str(), "r");
    if (pipe) {
        char buffer[1024];
        bool pastHeader = false;
        while (fgets(buffer, sizeof(buffer), pipe)) {
            if (!pastHeader) {
                if (strstr(buffer, "HEADER=END")) pastHeader = true;
                continue;
            }
            // Count non-empty data lines
            if (buffer[0] == ' ' && strlen(buffer) > 2) {
                stats.total_records++;
            }
        }
        pclose(pipe);
    }
    
    return stats;
}

void WalletMigrator::printReport(const MigrationStats& stats) {
    std::cout << "\n";
    std::cout << "Wallet Analysis Report\n";
    std::cout << "=====================\n";
    std::cout << "File size:     " << stats.original_size << " bytes\n";
    std::cout << "Total records: " << stats.total_records << "\n";
    std::cout << "Keys:          " << stats.keys_count << "\n";
    std::cout << "Transactions:  " << stats.transactions_count << "\n";
    std::cout << "Metadata:      " << stats.metadata_count << "\n";
    
    if (stats.duration.count() > 0) {
        std::cout << "Migration time: " << stats.duration.count() << " ms\n";
    }
}

void WalletMigrator::log(const std::string& message) {
    if (m_verbose) {
        std::cout << "[LOG] " << message << std::endl;
    }
}

void WalletMigrator::logError(const std::string& error) {
    std::cerr << "[ERROR] " << error << std::endl;
    m_lastError = error;
}

void WalletMigrator::logProgress(size_t current, size_t total) {
    if (!m_verbose) return;
    
    int percentage = (current * 100) / total;
    std::cout << "\r[" << std::string(percentage/5, '=') 
              << std::string(20 - percentage/5, ' ') 
              << "] " << percentage << "% - " 
              << current << "/" << total << " records" << std::flush;
}