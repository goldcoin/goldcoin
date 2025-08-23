#include "utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <cstdlib>
#include <sys/ioctl.h>
#include <unistd.h>

std::string Utils::calculateSHA256(const fs::path& file) {
    std::string cmd = "sha256sum \"" + file.string() + "\" 2>/dev/null | cut -d' ' -f1";
    return executeCommand(cmd);
}

bool Utils::isWalletLocked(const fs::path& walletPath) {
    // Check if wallet file is currently open by another process
    std::string cmd = "lsof \"" + walletPath.string() + "\" 2>/dev/null | wc -l";
    std::string result = executeCommand(cmd);
    return std::stoi(result) > 1;  // More than just the header line
}

std::string Utils::formatBytes(size_t bytes) {
    const char* units[] = {"B", "KB", "MB", "GB"};
    int unit = 0;
    double size = bytes;
    
    while (size >= 1024 && unit < 3) {
        size /= 1024;
        unit++;
    }
    
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << size << " " << units[unit];
    return ss.str();
}

std::string Utils::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

bool Utils::commandExists(const std::string& command) {
    std::string cmd = "which " + command + " > /dev/null 2>&1";
    return system(cmd.c_str()) == 0;
}

std::string Utils::executeCommand(const std::string& command) {
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return "";
    
    char buffer[128];
    std::string result;
    
    while (fgets(buffer, sizeof(buffer), pipe)) {
        result += buffer;
    }
    
    pclose(pipe);
    
    // Remove trailing newline
    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }
    
    return result;
}

int Utils::getTerminalWidth() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

void Utils::showProgress(size_t current, size_t total, const std::string& label) {
    if (total == 0) return;
    
    int width = getTerminalWidth() - 30;  // Leave room for percentage and numbers
    if (width < 20) width = 20;
    
    float progress = (float)current / total;
    int pos = width * progress;
    
    std::cout << "\r" << label;
    std::cout << "[";
    
    for (int i = 0; i < width; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    
    std::cout << "] " << int(progress * 100.0) << "% ";
    std::cout << current << "/" << total;
    std::cout << std::flush;
    
    if (current == total) {
        std::cout << std::endl;
    }
}