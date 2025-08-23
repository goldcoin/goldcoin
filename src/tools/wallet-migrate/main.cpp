// Goldcoin Wallet Migration Tool
// Standalone tool for migrating wallets from BDB 4.8 to BDB 18.1

#include <iostream>
#include <iomanip>
#include <chrono>
#include <cstring>
#include "wallet_migrate.h"
#include "wallet_analyzer.h"
#include "wallet_backup.h"

void printUsage(const char* program) {
    std::cout << "Goldcoin Wallet Migration Tool v0.1.0\n";
    std::cout << "Migrates wallets from Berkeley DB 4.8 to 18.1\n\n";
    std::cout << "Usage: " << program << " [OPTIONS] <wallet.dat>\n\n";
    std::cout << "Options:\n";
    std::cout << "  --analyze         Analyze wallet without making changes\n";
    std::cout << "  --migrate         Perform migration (creates backup first)\n";
    std::cout << "  --verify          Verify wallet integrity\n";
    std::cout << "  --dry-run         Simulate migration without changes\n";
    std::cout << "  --backup-dir DIR  Specify backup directory (default: same as wallet)\n";
    std::cout << "  --verbose         Enable verbose output\n";
    std::cout << "  --help            Show this help message\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << program << " --analyze ~/.goldcoin/wallet.dat\n";
    std::cout << "  " << program << " --migrate --verbose wallet.dat\n";
    std::cout << "  " << program << " --dry-run --backup-dir /backups wallet.dat\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printUsage(argv[0]);
        return 1;
    }

    // Parse command line arguments
    bool analyze = false;
    bool migrate = false;
    bool verify = false;
    bool dryRun = false;
    bool verbose = false;
    fs::path backupDir;
    fs::path walletPath;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--analyze") {
            analyze = true;
        } else if (arg == "--migrate") {
            migrate = true;
        } else if (arg == "--verify") {
            verify = true;
        } else if (arg == "--dry-run") {
            dryRun = true;
        } else if (arg == "--verbose") {
            verbose = true;
        } else if (arg == "--backup-dir" && i + 1 < argc) {
            backupDir = argv[++i];
        } else if (arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg[0] != '-') {
            walletPath = arg;
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            printUsage(argv[0]);
            return 1;
        }
    }

    // Validate wallet path
    if (walletPath.empty()) {
        std::cerr << "Error: No wallet file specified\n";
        printUsage(argv[0]);
        return 1;
    }

    if (!fs::exists(walletPath)) {
        std::cerr << "Error: Wallet file not found: " << walletPath << "\n";
        return 1;
    }

    // Create migrator instance
    WalletMigrator migrator;
    migrator.setVerbose(verbose);
    migrator.setDryRun(dryRun);
    
    if (!backupDir.empty()) {
        migrator.setBackupDir(backupDir);
    }

    // Print header
    std::cout << "========================================\n";
    std::cout << "  Goldcoin Wallet Migration Tool\n";
    std::cout << "  BDB 4.8 → BDB 18.1 Converter\n";
    std::cout << "========================================\n\n";

    // Detect wallet version
    std::cout << "Checking wallet: " << walletPath << "\n";
    WalletDBVersion version = migrator.detectVersion(walletPath);
    
    switch (version) {
        case WalletDBVersion::BDB_4_8:
            std::cout << "✓ Detected: Berkeley DB 4.8 (legacy format)\n";
            break;
        case WalletDBVersion::BDB_18_1:
            std::cout << "✓ Detected: Berkeley DB 18.1 (current format)\n";
            if (migrate) {
                std::cout << "! Wallet is already migrated\n";
                return 0;
            }
            break;
        default:
            std::cerr << "✗ Error: Unknown wallet format\n";
            return 1;
    }

    // Perform requested operation
    if (analyze) {
        std::cout << "\nAnalyzing wallet...\n";
        MigrationStats stats = migrator.analyze(walletPath);
        migrator.printReport(stats);
    }
    
    if (migrate) {
        if (version == WalletDBVersion::BDB_18_1) {
            std::cout << "Wallet is already in BDB 18.1 format\n";
            return 0;
        }
        
        std::cout << "\n" << (dryRun ? "DRY RUN - " : "") << "Starting migration...\n";
        
        if (!dryRun) {
            std::cout << "⚠️  WARNING: This will modify your wallet file.\n";
            std::cout << "A backup will be created automatically.\n";
            std::cout << "Continue? [y/N]: ";
            std::string response;
            std::getline(std::cin, response);
            if (response != "y" && response != "Y") {
                std::cout << "Migration cancelled.\n";
                return 0;
            }
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        MigrationResult result = migrator.migrate(walletPath);
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
        
        switch (result) {
            case MigrationResult::SUCCESS:
                std::cout << "\n✓ Migration completed successfully!\n";
                std::cout << "  Time: " << duration.count() << " seconds\n";
                break;
            case MigrationResult::USER_CANCELLED:
                std::cout << "Migration cancelled by user.\n";
                break;
            default:
                std::cerr << "\n✗ Migration failed: " << migrator.getLastError() << "\n";
                return 1;
        }
    }
    
    if (verify) {
        std::cout << "\nVerifying wallet integrity...\n";
        bool valid = migrator.verify(walletPath);
        if (valid) {
            std::cout << "✓ Wallet verification passed\n";
        } else {
            std::cerr << "✗ Wallet verification failed: " << migrator.getLastError() << "\n";
            return 1;
        }
    }

    std::cout << "\nDone.\n";
    return 0;
}