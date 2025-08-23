#pragma once

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class Utils {
public:
    // File utilities
    static std::string calculateSHA256(const fs::path& file);
    static bool isWalletLocked(const fs::path& walletPath);
    static std::string formatBytes(size_t bytes);
    static std::string getCurrentTimestamp();
    
    // System utilities  
    static bool commandExists(const std::string& command);
    static std::string executeCommand(const std::string& command);
    static int getTerminalWidth();
    
    // Progress bar
    static void showProgress(size_t current, size_t total, const std::string& label = "");
};