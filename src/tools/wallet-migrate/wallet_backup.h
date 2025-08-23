#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class WalletBackup {
public:
    // Backup management
    static bool createBackup(const fs::path& walletPath, const fs::path& backupDir = "");
    static std::vector<fs::path> listBackups(const fs::path& walletPath);
    static bool restoreBackup(const fs::path& backupPath, const fs::path& walletPath);
    static bool verifyBackup(const fs::path& backupPath);
    
private:
    static std::string generateBackupName(const fs::path& walletPath);
    static bool calculateChecksum(const fs::path& file, std::string& checksum);
};