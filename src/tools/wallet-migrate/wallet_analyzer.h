#pragma once

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class WalletAnalyzer {
public:
    // Detailed wallet analysis
    struct DetailedStats {
        size_t private_keys = 0;
        size_t public_keys = 0;
        size_t addresses = 0;
        size_t transactions = 0;
        size_t labels = 0;
        bool is_encrypted = false;
        bool is_hd_wallet = false;
        std::string creation_date;
        std::string last_modified;
    };
    
    static DetailedStats analyzeWallet(const fs::path& walletPath);
    static void printDetailedReport(const DetailedStats& stats);
};