// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GOLDCOIN_FS_H
#define GOLDCOIN_FS_H

/**
 * Filesystem operations and types
 * 
 * MIGRATION: Legacy filesystem → std::filesystem
 * This header provides a clean migration path from boost to std C++17/23
 */

#include <filesystem>
#include <system_error>

// Define fs namespace as std::filesystem (NO MORE BOOST!)
namespace fs = std::filesystem;

// Compatibility helpers for the migration
namespace fsbridge {

// Helper to convert paths to strings consistently
inline std::string PathToString(const fs::path& path) {
    return path.string();
}

// Helper to check if path is absolute (was is_complete in boost)
inline bool IsAbsolute(const fs::path& path) {
    return path.is_absolute();
}

// Create directories recursively
inline bool CreateDirectories(const fs::path& path) {
    std::error_code ec;
    return fs::create_directories(path, ec);
}

// Check if path exists
inline bool Exists(const fs::path& path) {
    std::error_code ec;
    return fs::exists(path, ec);
}

// Check if path is a directory
inline bool IsDirectory(const fs::path& path) {
    std::error_code ec;
    return fs::is_directory(path, ec);
}

// Remove file or directory
inline bool Remove(const fs::path& path) {
    std::error_code ec;
    return fs::remove(path, ec);
}

// Remove directory recursively
inline bool RemoveAll(const fs::path& path) {
    std::error_code ec;
    fs::remove_all(path, ec);
    return !ec;
}

// Get file size
inline uintmax_t FileSize(const fs::path& path) {
    std::error_code ec;
    auto size = fs::file_size(path, ec);
    return ec ? 0 : size;
}

// Rename/move file
inline bool Rename(const fs::path& old_path, const fs::path& new_path) {
    std::error_code ec;
    fs::rename(old_path, new_path, ec);
    return !ec;
}

} // namespace fsbridge

#endif // GOLDCOIN_FS_H