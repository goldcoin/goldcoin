// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <wallet/migrate.h>
#include <util.h>
#include <utiltime.h>

#include <fstream>
#include <cstring>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <vector>
#include <arpa/inet.h>  // For ntohl, ntohs
#include <sys/stat.h>   // For file permissions
#include <db_cxx.h>     // Berkeley DB C++ API

namespace WalletMigration {

// BDB magic bytes at offset 12
static const uint8_t BDB_BTREE_MAGIC[] = {0x62, 0x31, 0x05, 0x00};
static const uint8_t BDB48_VERSION = 0x09;  // at offset 16
static const uint8_t BDB18_VERSION = 0x0a;  // at offset 16

WalletDBVersion DetectWalletVersion(const fs::path& walletPath)
{
    LogPrintf("GOLDCOIN_DETECT_DEBUG: DetectWalletVersion called with path='%s'\n", walletPath.string());
    
    std::ifstream file(walletPath, std::ios::binary);
    if (!file) {
        LogPrintf("GOLDCOIN_DETECT_DEBUG: ERROR - Cannot open file\n");
        return WalletDBVersion::UNKNOWN;
    }
    
    // Read header
    uint8_t header[32];
    file.read(reinterpret_cast<char*>(header), sizeof(header));
    
    LogPrintf("GOLDCOIN_DETECT_DEBUG: Read %d bytes\n", (int)file.gcount());
    
    if (file.gcount() >= 17) {
        // Check BDB magic at offset 12
        LogPrintf("GOLDCOIN_DETECT_DEBUG: Magic bytes at offset 12: %02x %02x %02x %02x\n", 
                 header[12], header[13], header[14], header[15]);
        
        if (std::memcmp(header + 12, BDB_BTREE_MAGIC, 4) == 0) {
            LogPrintf("GOLDCOIN_DETECT_DEBUG: BDB magic MATCH!\n");
            uint8_t version = header[16];
            LogPrintf("GOLDCOIN_DETECT_DEBUG: Version byte at offset 16: 0x%02x\n", version);
            
            if (version == BDB48_VERSION) {
                LogPrintf("GOLDCOIN_DETECT_DEBUG: DETECTED BDB 4.8 - returning BDB_4_8\n");
                return WalletDBVersion::BDB_4_8;
            } else if (version == BDB18_VERSION) {
                LogPrintf("GOLDCOIN_DETECT_DEBUG: DETECTED BDB 18.1 - returning BDB_18_1\n");
                return WalletDBVersion::BDB_18_1;
            } else {
                LogPrintf("GOLDCOIN_DETECT_DEBUG: Unknown BDB version: 0x%02x\n", version);
            }
        } else {
            LogPrintf("GOLDCOIN_DETECT_DEBUG: BDB magic MISMATCH!\n");
        }
    } else {
        LogPrintf("GOLDCOIN_DETECT_DEBUG: File too short: %d bytes\n", (int)file.gcount());
    }
    
    LogPrintf("GOLDCOIN_DETECT_DEBUG: Returning UNKNOWN\n");
    return WalletDBVersion::UNKNOWN;
}

bool NeedsMigration(const fs::path& walletPath)
{
    return DetectWalletVersion(walletPath) == WalletDBVersion::BDB_4_8;
}

fs::path CreateMigrationBackup(const fs::path& walletPath)
{
    // Generate timestamp
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream timestamp;
    timestamp << std::put_time(std::localtime(&time_t), "%Y%m%d-%H%M%S");
    
    // Create backup filename in same directory
    fs::path backupPath = walletPath.parent_path() / 
        (walletPath.filename().string() + ".bdb48-backup-" + timestamp.str());
    
    try {
        fs::copy_file(walletPath, backupPath, fs::copy_options::overwrite_existing);
        
        // Verify backup
        if (fs::exists(backupPath) && 
            fs::file_size(backupPath) == fs::file_size(walletPath)) {
            LogPrintf("Wallet backup created: %s\n", backupPath.string());
            return backupPath;
        }
    } catch (const fs::filesystem_error& e) {
        LogPrintf("Failed to create wallet backup: %s\n", e.what());
    }
    
    return fs::path(); // empty path on failure
}

// BDB 4.8 Page Types
enum PageType {
    P_INVALID    = 0,
    P_DUPLICATE  = 1,
    P_HASH       = 2,
    P_IBTREE     = 3,
    P_IRECNO     = 4,
    P_LBTREE     = 5,
    P_LRECNO     = 6,
    P_OVERFLOW   = 7,
    P_HASHMETA   = 8,
    P_BTREEMETA  = 9,
    P_QAMMETA    = 10,
    P_QAMDATA    = 11,
    P_LDUP       = 12,
    P_HASH_UNSORTED = 13
};

// BDB 4.8 Page Header (all pages start with this)
struct PageHeader {
    uint32_t lsn_file;     // Log sequence number file
    uint32_t lsn_offset;   // Log sequence number offset  
    uint32_t pgno;         // Page number
    uint32_t prev_pgno;    // Previous page
    uint32_t next_pgno;    // Next page
    uint16_t entries;      // Number of items on page
    uint16_t hf_offset;    // High free byte page offset
    uint8_t  level;        // Btree level (0=leaf)
    uint8_t  type;         // Page type (PageType enum)
};

// Btree metadata page (page 0)
struct BtreeMetaData {
    PageHeader header;
    uint32_t magic;        // Btree magic number
    uint32_t version;      // Version
    uint32_t pagesize;     // Page size
    uint8_t  encrypt_alg;  // Encryption algorithm
    uint8_t  page_type;    // Page type  
    uint8_t  metaflags;    // Metadata flags
    uint8_t  unused1;      // Unused
    uint32_t free;         // Free list page number
    uint32_t last_pgno;    // Last page in database
    uint32_t nparts;       // Number of partitions
    uint32_t key_count;    // Cached key count
    uint32_t record_count; // Cached record count
    uint32_t flags;        // Flags
    uint8_t  uid[20];      // Unique file ID
};

// Read a value from buffer with endianness handling
template<typename T>
T ReadValue(const uint8_t* buffer, size_t offset) {
    T value;
    memcpy(&value, buffer + offset, sizeof(T));
    // BDB uses big-endian for multi-byte values
    if (sizeof(T) == 2) {
        value = ntohs(value);
    } else if (sizeof(T) == 4) {
        value = ntohl(value);
    }
    return value;
}

// Simple BDB 4.8 reader - extracts key-value pairs
class BDB48Reader {
private:
    std::ifstream file;
    uint32_t pageSize;
    std::vector<std::pair<std::vector<uint8_t>, std::vector<uint8_t>>> records;
    
    bool ReadPage(uint32_t pgno, std::vector<uint8_t>& page) {
        if (pgno == 0 || pgno == 0xFFFFFFFF) return false;
        file.seekg(pgno * pageSize);
        page.resize(pageSize);
        file.read(reinterpret_cast<char*>(page.data()), pageSize);
        return file.good();
    }
    
    // Handle overflow pages for large records
    std::vector<uint8_t> ReadOverflowData(uint32_t pgno, uint32_t totalLen) {
        std::vector<uint8_t> data;
        data.reserve(totalLen);
        
        while (pgno != 0 && data.size() < totalLen) {
            std::vector<uint8_t> page;
            if (!ReadPage(pgno, page)) break;
            
            // Overflow page header
            PageHeader header;
            memcpy(&header, page.data(), sizeof(PageHeader));
            
            // Get next overflow page
            pgno = ntohl(header.next_pgno);
            
            // Copy data from this page (after header)
            size_t dataStart = sizeof(PageHeader);
            size_t dataLen = std::min(size_t(totalLen - data.size()), 
                                     pageSize - dataStart);
            data.insert(data.end(), 
                       page.begin() + dataStart, 
                       page.begin() + dataStart + dataLen);
        }
        
        return data;
    }
    
    void ProcessBtreePage(const std::vector<uint8_t>& page) {
        PageHeader header;
        memcpy(&header, page.data(), sizeof(PageHeader));
        header.pgno = ntohl(header.pgno);
        header.entries = ntohs(header.entries);
        header.hf_offset = ntohs(header.hf_offset);
        
        // Only process leaf pages (level 0)
        if (header.level != 0) return;
        
        // Btree leaf page entries are pairs of key-value records
        // Index starts after the page header
        size_t indexStart = sizeof(PageHeader);
        
        // Process each entry in the page
        for (uint16_t i = 0; i < header.entries; i += 2) {
            // BDB btree stores key/value pairs, so we read in pairs
            if (i + 1 >= header.entries) break;
            
            // Get offsets for key and value
            uint16_t keyOffset = ReadValue<uint16_t>(page.data(), indexStart + (i * 2));
            uint16_t valOffset = ReadValue<uint16_t>(page.data(), indexStart + ((i + 1) * 2));
            
            if (keyOffset >= pageSize || valOffset >= pageSize) continue;
            
            // Read key data
            size_t keyPos = keyOffset;
            if (keyPos + 3 > pageSize) continue;
            
            // BDB record format: [len:2][data:len] for simple data records
            uint16_t keyLen = ReadValue<uint16_t>(page.data(), keyPos);
            keyPos += 2;
            
            if (keyPos + keyLen > pageSize) continue;
            std::vector<uint8_t> key(page.begin() + keyPos, page.begin() + keyPos + keyLen);
            
            // Read value data
            size_t valPos = valOffset;
            if (valPos + 3 > pageSize) continue;
            
            uint16_t valLen = ReadValue<uint16_t>(page.data(), valPos);
            valPos += 2;
            
            if (valPos + valLen > pageSize) continue;
            std::vector<uint8_t> value(page.begin() + valPos, page.begin() + valPos + valLen);
            
            // Store the key-value pair
            if (!key.empty() && !value.empty()) {
                records.push_back({key, value});
            }
        }
    }
    
public:
    bool Open(const fs::path& walletPath) {
        file.open(walletPath, std::ios::binary);
        if (!file) return false;
        
        // First read metadata to get page size
        uint8_t metaBuffer[512];
        file.read(reinterpret_cast<char*>(metaBuffer), sizeof(metaBuffer));
        if (!file.good()) return false;
        
        BtreeMetaData meta;
        memcpy(&meta, metaBuffer, sizeof(BtreeMetaData));
        
        // Verify magic at correct offset (12 from file start)
        if (memcmp(metaBuffer + 12, BDB_BTREE_MAGIC, 4) != 0) {
            LogPrintf("GOLDCOIN_MIGRATE_DEBUG: Magic mismatch - expected 62 31 05 00, got %02x %02x %02x %02x\n",
                     metaBuffer[12], metaBuffer[13], metaBuffer[14], metaBuffer[15]);
            return false;
        }
        
        pageSize = ntohl(meta.pagesize);
        if (pageSize == 0 || pageSize > 65536) {
            return false;
        }
        
        uint32_t lastPage = ntohl(meta.last_pgno);
        
        // Read all pages and extract records
        for (uint32_t pgno = 1; pgno <= lastPage; pgno++) {
            std::vector<uint8_t> page;
            if (!ReadPage(pgno, page)) continue;
            
            // Check page type
            uint8_t pageType = page[sizeof(PageHeader) - 1];
            if (pageType == P_LBTREE) {
                ProcessBtreePage(page);
            }
        }
        
        return true;
    }
    
    const std::vector<std::pair<std::vector<uint8_t>, std::vector<uint8_t>>>& GetRecords() const {
        return records;
    }
};

bool MigrateWallet(const fs::path& walletPath)
{
    LogPrintf("GOLDCOIN_MIGRATE_DEBUG: *** STARTING WALLET MIGRATION ***\n");
    LogPrintf("GOLDCOIN_MIGRATE_DEBUG: Wallet path: %s\n", walletPath.string().c_str());
    LogPrintf("GOLDCOIN_MIGRATE_DEBUG: Wallet size: %lu bytes\n", fs::file_size(walletPath));
    
    LogPrintf("Starting wallet migration for %s\n", walletPath.string());
    LogPrintf("Wallet size: %lu bytes\n", fs::file_size(walletPath));
    
    // Step 1: Create backup
    fs::path backupPath = CreateMigrationBackup(walletPath);
    if (backupPath.empty()) {
        LogPrintf("ERROR: Failed to create backup\n");
        return false;
    }
    
    // Step 2: Read BDB 4.8 wallet using custom reader
    LogPrintf("Reading BDB 4.8 wallet data...\n");
    BDB48Reader reader;
    if (!reader.Open(walletPath)) {
        LogPrintf("ERROR: Failed to open BDB 4.8 wallet\n");
        fs::remove(backupPath);
        return false;
    }
    
    const auto& records = reader.GetRecords();
    LogPrintf("Successfully read %lu records from BDB 4.8 wallet\n", records.size());
    
    if (records.empty()) {
        LogPrintf("ERROR: No records found in wallet (corrupt or empty)\n");
        fs::remove(backupPath);
        return false;
    }
    
    // Log some statistics for debugging
    size_t totalKeySize = 0, totalValueSize = 0;
    for (const auto& [key, value] : records) {
        totalKeySize += key.size();
        totalValueSize += value.size();
    }
    LogPrintf("Migration stats: %lu records, %lu bytes of keys, %lu bytes of values\n",
              records.size(), totalKeySize, totalValueSize);
    
    // Step 3: Write to new BDB 18.1 wallet
    fs::path tempPath = walletPath.string() + ".migrating";
    
    // Create new BDB 18.1 database environment
    // We'll write raw to avoid wallet class dependencies
    try {
        // Remove any existing temp file
        if (fs::exists(tempPath)) {
            fs::remove(tempPath);
        }
        
        // Create new BDB environment for the migrated wallet
        LogPrintf("GOLDCOIN_MIGRATE_DEBUG: Creating new BDB 18.1 environment...\n");
        DbEnv dbenv(DB_CXX_NO_EXCEPTIONS);
        dbenv.set_lg_dir(".");
        dbenv.set_cachesize(0, 0x100000, 1); // 1MB cache
        dbenv.set_lg_bsize(0x10000);
        dbenv.set_lg_max(1048576);
        dbenv.set_lk_max_locks(40000);
        dbenv.set_lk_max_objects(40000);
        dbenv.set_errfile(fopen("/dev/null", "a"));
        dbenv.set_flags(DB_AUTO_COMMIT, 1);
        dbenv.set_flags(DB_TXN_WRITE_NOSYNC, 1);
        dbenv.log_set_config(DB_LOG_AUTO_REMOVE, 1);
        
        LogPrintf("GOLDCOIN_MIGRATE_DEBUG: Opening BDB environment at: %s\n", 
               walletPath.parent_path().string().c_str());
        
        // Open environment
        int ret = dbenv.open(walletPath.parent_path().string().c_str(),
                           DB_CREATE |
                           DB_INIT_LOCK |
                           DB_INIT_LOG |
                           DB_INIT_MPOOL |
                           DB_INIT_TXN |
                           DB_THREAD |
                           DB_PRIVATE,
                           S_IRUSR | S_IWUSR);
        
        if (ret != 0) {
            LogPrintf("GOLDCOIN_MIGRATE_DEBUG: ERROR - BDB environment open failed: %s\n", DbEnv::strerror(ret));
            LogPrintf("Failed to create BDB environment: %s\n", DbEnv::strerror(ret));
            fs::remove(backupPath);
            return false;
        }
        LogPrintf("GOLDCOIN_MIGRATE_DEBUG: BDB environment opened successfully\n");
        
        // Create the new database
        LogPrintf("GOLDCOIN_MIGRATE_DEBUG: Creating new database file: %s\n", tempPath.filename().string().c_str());
        Db db(&dbenv, 0);
        
        ret = db.open(nullptr,                    // Txn pointer
                     tempPath.filename().string().c_str(), // Filename
                     "main",                       // Logical db name
                     DB_BTREE,                     // Database type
                     DB_CREATE,                    // Flags
                     0);
        
        if (ret != 0) {
            LogPrintf("GOLDCOIN_MIGRATE_DEBUG: ERROR - Database creation failed: %s\n", DbEnv::strerror(ret));
            LogPrintf("Failed to create new wallet database: %s\n", DbEnv::strerror(ret));
            dbenv.close(0);
            fs::remove(backupPath);
            return false;
        }
        LogPrintf("GOLDCOIN_MIGRATE_DEBUG: Database created successfully\n");
        
        // Write all records to new database
        LogPrintf("GOLDCOIN_MIGRATE_DEBUG: Writing %lu records to new database...\n", records.size());
        size_t successCount = 0, failCount = 0;
        
        for (const auto& [key, value] : records) {
            Dbt datKey(const_cast<uint8_t*>(key.data()), key.size());
            Dbt datValue(const_cast<uint8_t*>(value.data()), value.size());
            
            int put_ret = db.put(nullptr, &datKey, &datValue, 0);
            if (put_ret != 0) {
                failCount++;
                LogPrintf("GOLDCOIN_MIGRATE_DEBUG: Write failed for record (key size=%lu): %s\n", 
                       key.size(), DbEnv::strerror(put_ret));
                LogPrintf("Warning: Failed to write record: %s\n", DbEnv::strerror(put_ret));
            } else {
                successCount++;
            }
        }
        
        LogPrintf("GOLDCOIN_MIGRATE_DEBUG: Write complete. Success: %lu, Failed: %lu\n", 
               successCount, failCount);
        
        // Close database and environment
        db.close(0);
        dbenv.close(0);
        
    } catch (const DbException& e) {
        LogPrintf("Database exception during migration: %s\n", e.what());
        fs::remove(backupPath);
        if (fs::exists(tempPath)) {
            fs::remove(tempPath);
        }
        return false;
    } catch (...) {
        LogPrintf("Unknown exception during migration\n");
        fs::remove(backupPath);
        if (fs::exists(tempPath)) {
            fs::remove(tempPath);
        }
        return false;
    }
    
    // Step 4: Verify the migrated wallet is BDB 18.1
    LogPrintf("GOLDCOIN_MIGRATE_DEBUG: Verifying migrated wallet format...\n");
    WalletDBVersion migratedVersion = DetectWalletVersion(tempPath);
    LogPrintf("GOLDCOIN_MIGRATE_DEBUG: Detected migrated wallet version: %d\n", (int)migratedVersion);
    
    if (migratedVersion != WalletDBVersion::BDB_18_1) {
        LogPrintf("GOLDCOIN_MIGRATE_DEBUG: ERROR - Migration verification failed. Expected BDB 18.1, got %d\n", 
               (int)migratedVersion);
        LogPrintf("Migration verification failed - not BDB 18.1 format\n");
        fs::remove(tempPath);
        fs::remove(backupPath);
        return false;
    }
    LogPrintf("GOLDCOIN_MIGRATE_DEBUG: Migration verification PASSED - wallet is BDB 18.1\n");
    
    // Step 5: Atomic replacement
    LogPrintf("GOLDCOIN_MIGRATE_DEBUG: Performing atomic wallet replacement...\n");
    try {
        // Move original aside temporarily
        fs::path oldPath = walletPath.string() + ".old";
        LogPrintf("GOLDCOIN_MIGRATE_DEBUG: Moving original wallet to: %s\n", oldPath.string().c_str());
        fs::rename(walletPath, oldPath);
        
        // Move new wallet into place
        LogPrintf("GOLDCOIN_MIGRATE_DEBUG: Moving migrated wallet to: %s\n", walletPath.string().c_str());
        fs::rename(tempPath, walletPath);
        
        // Remove old wallet
        LogPrintf("GOLDCOIN_MIGRATE_DEBUG: Removing old wallet file\n");
        fs::remove(oldPath);
        
        LogPrintf("GOLDCOIN_MIGRATE_DEBUG: *** MIGRATION COMPLETED SUCCESSFULLY ***\n");
        LogPrintf("Wallet migrated successfully (backup: %s)\n", backupPath.filename().string());
        return true;
        
    } catch (const fs::filesystem_error& e) {
        LogPrintf("Failed to replace wallet: %s\n", e.what());
        
        // Try to restore
        if (fs::exists(backupPath)) {
            try {
                if (fs::exists(walletPath)) {
                    fs::remove(walletPath);
                }
                fs::copy_file(backupPath, walletPath);
            } catch (...) {
                LogPrintf("Failed to restore backup!\n");
            }
        }
        return false;
    }
}

} // namespace WalletMigration