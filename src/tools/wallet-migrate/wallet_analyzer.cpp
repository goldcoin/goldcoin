#include "wallet_analyzer.h"
#include <iostream>
#include <ctime>

WalletAnalyzer::DetailedStats WalletAnalyzer::analyzeWallet(const fs::path& walletPath) {
    DetailedStats stats;
    
    // Get file timestamps
    auto ftime = fs::last_write_time(walletPath);
    auto cftime = std::chrono::system_clock::to_time_t(
        std::chrono::file_clock::to_sys(ftime));
    stats.last_modified = std::ctime(&cftime);
    
    // TODO: Implement detailed analysis using db_dump parsing
    
    return stats;
}

void WalletAnalyzer::printDetailedReport(const DetailedStats& stats) {
    std::cout << "\nDetailed Wallet Analysis\n";
    std::cout << "========================\n";
    std::cout << "Private keys:    " << stats.private_keys << "\n";
    std::cout << "Public keys:     " << stats.public_keys << "\n";
    std::cout << "Addresses:       " << stats.addresses << "\n";
    std::cout << "Transactions:    " << stats.transactions << "\n";
    std::cout << "Labels:          " << stats.labels << "\n";
    std::cout << "Encrypted:       " << (stats.is_encrypted ? "Yes" : "No") << "\n";
    std::cout << "HD Wallet:       " << (stats.is_hd_wallet ? "Yes" : "No") << "\n";
    std::cout << "Last modified:   " << stats.last_modified;
}