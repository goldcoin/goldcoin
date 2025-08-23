#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

// BDB 4.8 compatibility layer
// This provides a way to read BDB 4.8 files without the library
class BDB48Compat {
public:
    // Alternative methods for reading BDB 4.8
    enum Method {
        USE_DB_DUMP,     // Use db4.8_dump utility (most reliable)
        PARSE_BINARY,    // Direct binary parsing (complex)
        PYTHON_BRIDGE    // Use Python bsddb3 module
    };
    
    static bool isAvailable(Method method);
    static Method getBestMethod();
    
    // Export wallet data to intermediate format
    static bool exportWallet(const fs::path& walletPath, 
                           const fs::path& exportPath,
                           Method method = USE_DB_DUMP);
    
    // Import from intermediate format to BDB 18.1
    static bool importWallet(const fs::path& exportPath,
                           const fs::path& walletPath);
    
private:
    static bool exportUsingDbDump(const fs::path& wallet, const fs::path& output);
    static bool exportUsingBinaryParser(const fs::path& wallet, const fs::path& output);
    static bool exportUsingPython(const fs::path& wallet, const fs::path& output);
};