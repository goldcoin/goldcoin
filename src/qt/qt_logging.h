// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GOLDCOIN_QT_LOGGING_H
#define GOLDCOIN_QT_LOGGING_H

//! C++23 Modern Logging for Qt
//! Replaces QString::arg with std::format

#include <format>
// #include <print>  // Not supported in MinGW GCC 13
#include "qt_modernization.h"  // For std::print compatibility
#include <string>
#include <string_view>
#include <QString>
#include <QDebug>
#include <chrono>
#include <source_location>

namespace goldcoin::qt::logging {

// Convert QString to std::string_view for formatting
inline std::string to_std_string(const QString& qs) {
    return qs.toStdString();
}

// C++23 formatted QString creation
template<typename... Args>
QString format_qt(std::string_view fmt, Args&&... args) {
    return QString::fromStdString(std::format(fmt, std::forward<Args>(args)...));
}

// C++23 print with Qt integration
template<typename... Args>
void print_qt(std::string_view fmt, Args&&... args) {
    std::print("{}", std::format(fmt, std::forward<Args>(args)...));
}

// Debug logging with source location
template<typename... Args>
void debug_qt(std::string_view fmt, Args&&... args, 
              const std::source_location& loc = std::source_location::current()) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    
    std::print("[{:%Y-%m-%d %H:%M:%S}] [{}:{}] {}\n",
               std::chrono::floor<std::chrono::seconds>(now),
               loc.file_name(), loc.line(),
               std::format(fmt, std::forward<Args>(args)...));
}

// Replace old QString::arg patterns
class ModernQString {
    QString str;
    
public:
    explicit ModernQString(const QString& s) : str(s) {}
    
    // C++23 style formatting
    template<typename... Args>
    static QString format(std::string_view fmt, Args&&... args) {
        return QString::fromStdString(std::format(fmt, std::forward<Args>(args)...));
    }
    
    // Chain-able operations using C++23 deducing this
    template<typename Self>
    auto&& append(this Self&& self, std::string_view text) {
        self.str.append(QString::fromStdString(std::string(text)));
        return std::forward<Self>(self);
    }
    
    operator QString() const { return str; }
};

// Macros for easy migration from old code
#define QT_FORMAT(fmt, ...) goldcoin::qt::logging::format_qt(fmt, __VA_ARGS__)
#define QT_PRINT(fmt, ...) goldcoin::qt::logging::print_qt(fmt, __VA_ARGS__)
#define QT_DEBUG(fmt, ...) goldcoin::qt::logging::debug_qt(fmt, __VA_ARGS__)

// Performance timer using C++23 chrono
class PerformanceTimer {
    using clock = std::chrono::high_resolution_clock;
    using time_point = clock::time_point;
    
    time_point start;
    std::string_view name;
    
public:
    explicit PerformanceTimer(std::string_view n) : start(clock::now()), name(n) {}
    
    ~PerformanceTimer() {
        auto end = clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::print("[PERF] {} took {} μs\n", name, duration.count());
    }
};

#define QT_PERF_TIMER(name) goldcoin::qt::logging::PerformanceTimer _timer(name)

} // namespace goldcoin::qt::logging

#endif // GOLDCOIN_QT_LOGGING_H