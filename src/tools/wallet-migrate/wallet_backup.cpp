#include "wallet_backup.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

bool WalletBackup::createBackup(const fs::path& walletPath, const fs::path& backupDir) {
    fs::path targetDir = backupDir.empty() ? walletPath.parent_path() : backupDir;
    std::string backupName = generateBackupName(walletPath);
    fs::path backupPath = targetDir / backupName;
    
    try {
        fs::copy_file(walletPath, backupPath, fs::copy_options::overwrite_existing);
        return fs::exists(backupPath);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Backup error: " << e.what() << std::endl;
        return false;
    }
}

std::string WalletBackup::generateBackupName(const fs::path& walletPath) {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << walletPath.stem().string() 
       << ".backup-"
       << std::put_time(std::localtime(&time_t), "%Y%m%d-%H%M%S")
       << walletPath.extension().string();
    
    return ss.str();
}

std::vector<fs::path> WalletBackup::listBackups(const fs::path& walletPath) {
    std::vector<fs::path> backups;
    std::string pattern = walletPath.stem().string() + ".backup-";
    
    for (const auto& entry : fs::directory_iterator(walletPath.parent_path())) {
        if (entry.path().filename().string().find(pattern) == 0) {
            backups.push_back(entry.path());
        }
    }
    
    return backups;
}

bool WalletBackup::restoreBackup(const fs::path& backupPath, const fs::path& walletPath) {
    try {
        fs::copy_file(backupPath, walletPath, fs::copy_options::overwrite_existing);
        return true;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Restore error: " << e.what() << std::endl;
        return false;
    }
}

bool WalletBackup::verifyBackup(const fs::path& backupPath) {
    return fs::exists(backupPath) && fs::file_size(backupPath) > 0;
}