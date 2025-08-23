#include "wallet_compat.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

bool BDB48Compat::isAvailable(Method method) {
    switch (method) {
        case USE_DB_DUMP:
            // Check if db4.8_dump is available
            return system("which db4.8_dump > /dev/null 2>&1") == 0 ||
                   system("which db48_dump > /dev/null 2>&1") == 0;
        
        case PYTHON_BRIDGE:
            // Check if Python with bsddb3 is available
            return system("python3 -c 'import bsddb3' 2>/dev/null") == 0;
        
        case PARSE_BINARY:
            // Binary parser is always available (built-in)
            return true;
        
        default:
            return false;
    }
}

BDB48Compat::Method BDB48Compat::getBestMethod() {
    // Prefer db_dump for reliability
    if (isAvailable(USE_DB_DUMP)) {
        std::cout << "Using db4.8_dump method (most reliable)\n";
        return USE_DB_DUMP;
    }
    
    // Fallback to Python if available
    if (isAvailable(PYTHON_BRIDGE)) {
        std::cout << "Using Python bsddb3 bridge\n";
        return PYTHON_BRIDGE;
    }
    
    // Last resort: binary parser
    std::cout << "Using binary parser (experimental)\n";
    return PARSE_BINARY;
}

bool BDB48Compat::exportWallet(const fs::path& walletPath, 
                              const fs::path& exportPath,
                              Method method) {
    switch (method) {
        case USE_DB_DUMP:
            return exportUsingDbDump(walletPath, exportPath);
        case PYTHON_BRIDGE:
            return exportUsingPython(walletPath, exportPath);
        case PARSE_BINARY:
            return exportUsingBinaryParser(walletPath, exportPath);
        default:
            return false;
    }
}

bool BDB48Compat::exportUsingDbDump(const fs::path& wallet, const fs::path& output) {
    // Try different possible db_dump commands
    std::vector<std::string> commands = {
        "/home/microguy/git/microguy/goldcoin/depends/work/build/x86_64-pc-linux-gnu/bdb48-utils/4.8.30.NC-638779dad17/build_unix/db_dump",  // Depends-built BDB 4.8 (correct hash)
        "/usr/local/bin/db_dump",  // Fallback to system BDB 4.8.30
        "db_dump",                  // Fallback to PATH
        "db4.8_dump",
        "db48_dump"
    };
    
    for (const auto& cmd : commands) {
        std::string fullCmd = cmd + " \"" + wallet.string() + "\" > \"" + output.string() + "\" 2>/dev/null";
        
        std::cerr << "Trying: " << cmd << std::endl;
        if (system(fullCmd.c_str()) == 0) {
            // Verify output was created
            if (fs::exists(output) && fs::file_size(output) > 0) {
                std::cerr << "Success with: " << cmd << std::endl;
                return true;
            }
        }
    }
    
    std::cerr << "Failed to export using db_dump\n";
    std::cerr << "Please install Berkeley DB 4.8 utilities:\n";
    std::cerr << "  Ubuntu/Debian: apt-get install db4.8-util\n";
    std::cerr << "  Or compile from source: https://www.oracle.com/database/technologies/related/berkeleydb-downloads.html\n";
    return false;
}

bool BDB48Compat::exportUsingPython(const fs::path& wallet, const fs::path& output) {
    // Python script to export BDB 4.8 using bsddb3
    std::string pythonScript = R"(
import sys
import bsddb3
import json

try:
    db = bsddb3.hashopen(sys.argv[1], 'r')
    data = {}
    for key in db.keys():
        data[key.hex()] = db[key].hex()
    
    with open(sys.argv[2], 'w') as f:
        json.dump(data, f)
    
    db.close()
    sys.exit(0)
except Exception as e:
    print(f"Error: {e}", file=sys.stderr)
    sys.exit(1)
)";
    
    // Write temporary Python script
    fs::path scriptPath = "/tmp/wallet_export.py";
    std::ofstream script(scriptPath);
    script << pythonScript;
    script.close();
    
    // Execute Python script
    std::string cmd = "python3 " + scriptPath.string() + " \"" + 
                     wallet.string() + "\" \"" + output.string() + "\" 2>/dev/null";
    
    int result = system(cmd.c_str());
    
    // Clean up
    fs::remove(scriptPath);
    
    return result == 0 && fs::exists(output);
}

bool BDB48Compat::exportUsingBinaryParser(const fs::path& wallet, const fs::path& output) {
    // This would implement direct binary parsing of BDB 4.8 format
    // For now, this is a placeholder - would need significant work
    
    std::cerr << "Binary parser not yet implemented\n";
    std::cerr << "This would require parsing BDB 4.8 btree format directly\n";
    std::cerr << "Please use db4.8_dump or Python bsddb3 method instead\n";
    
    return false;
}

bool BDB48Compat::importWallet(const fs::path& exportPath, const fs::path& walletPath) {
    // Import using db_load (BDB 18.1)
    std::string cmd = "/home/microguy/build/bdb-18.1-utils/db-18.1.40/build_unix/db_load -f \"" + 
                     exportPath.string() + "\" \"" + 
                     walletPath.string() + "\" 2>/dev/null";
    
    return system(cmd.c_str()) == 0;
}