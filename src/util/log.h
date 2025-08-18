// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GOLDCOIN_UTIL_LOG_H
#define GOLDCOIN_UTIL_LOG_H

#include <print>
#include <format>
#include <chrono>
#include <source_location>
#include <stacktrace>
#include <fstream>
#include <mutex>
#include <string_view>

namespace goldcoin {

// C++23 Modern Logging System - NO MORE printf!
class Logger {
public:
    enum class Level {
        DEBUG = 0,
        INFO = 1,
        WARNING = 2,
        ERROR = 3,
        CRITICAL = 4
    };

private:
    Level m_min_level = Level::INFO;
    std::mutex m_mutex;
    std::ofstream m_file;
    bool m_console_output = true;
    bool m_file_output = false;

    static constexpr std::string_view LevelString(Level level) {
        switch(level) {
            case Level::DEBUG: return "DEBUG";
            case Level::INFO: return "INFO";
            case Level::WARNING: return "WARN";
            case Level::ERROR: return "ERROR";
            case Level::CRITICAL: return "CRIT";
        }
        return "UNKNOWN";
    }

    static constexpr std::string_view LevelColor(Level level) {
        switch(level) {
            case Level::DEBUG: return "\033[36m";    // Cyan
            case Level::INFO: return "\033[32m";     // Green
            case Level::WARNING: return "\033[33m";  // Yellow
            case Level::ERROR: return "\033[31m";    // Red
            case Level::CRITICAL: return "\033[35m"; // Magenta
        }
        return "\033[0m";
    }

public:
    static Logger& Instance() {
        static Logger instance;
        return instance;
    }

    void SetLevel(Level level) { m_min_level = level; }
    void EnableFileLogging(const std::string& filename) {
        std::lock_guard lock(m_mutex);
        m_file.open(filename, std::ios::app);
        m_file_output = m_file.is_open();
    }

    // C++23 std::print logging with source location
    template<typename... Args>
    void Log(Level level, std::format_string<Args...> fmt, Args&&... args,
             const std::source_location& loc = std::source_location::current()) {
        
        if (level < m_min_level) return;

        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

        std::string message = std::format(fmt, std::forward<Args>(args)...);
        
        // Format: [2025-08-18 15:30:45.123] [INFO] [file.cpp:42] Message
        std::string log_line = std::format(
            "[{:%Y-%m-%d %H:%M:%S}.{:03d}] [{}] [{}:{}] {}",
            std::chrono::floor<std::chrono::seconds>(now),
            ms.count(),
            LevelString(level),
            loc.file_name(),
            loc.line(),
            message
        );

        std::lock_guard lock(m_mutex);
        
        if (m_console_output) {
            // Color output for console
            std::println("{}{}{}\033[0m", LevelColor(level), log_line, "");
        }
        
        if (m_file_output && m_file.is_open()) {
            std::println(m_file, "{}", log_line);
            m_file.flush();
        }
    }

    // Log with stack trace for errors
    template<typename... Args>
    void LogError(std::format_string<Args...> fmt, Args&&... args) {
        Log(Level::ERROR, fmt, std::forward<Args>(args)...);
        if (m_min_level <= Level::DEBUG) {
            auto trace = std::stacktrace::current();
            std::println(stderr, "Stack trace:\n{}", std::to_string(trace));
        }
    }

    // Specialized methods for each level
    template<typename... Args>
    void Debug(std::format_string<Args...> fmt, Args&&... args) {
        Log(Level::DEBUG, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Info(std::format_string<Args...> fmt, Args&&... args) {
        Log(Level::INFO, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Warning(std::format_string<Args...> fmt, Args&&... args) {
        Log(Level::WARNING, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Error(std::format_string<Args...> fmt, Args&&... args) {
        LogError(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Critical(std::format_string<Args...> fmt, Args&&... args) {
        Log(Level::CRITICAL, fmt, std::forward<Args>(args)...);
        // For critical errors, also dump stack trace
        auto trace = std::stacktrace::current();
        std::println(stderr, "Critical error stack trace:\n{}", std::to_string(trace));
    }
};

// Global logging macros using C++23 features
#define LOG_DEBUG(...) goldcoin::Logger::Instance().Debug(__VA_ARGS__)
#define LOG_INFO(...) goldcoin::Logger::Instance().Info(__VA_ARGS__)
#define LOG_WARNING(...) goldcoin::Logger::Instance().Warning(__VA_ARGS__)
#define LOG_ERROR(...) goldcoin::Logger::Instance().Error(__VA_ARGS__)
#define LOG_CRITICAL(...) goldcoin::Logger::Instance().Critical(__VA_ARGS__)

// Benchmark logging using C++23 chrono
class BenchmarkLogger {
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;
    
    std::string m_name;
    TimePoint m_start;
    
public:
    explicit BenchmarkLogger(std::string name) 
        : m_name(std::move(name)), m_start(Clock::now()) {
        LOG_DEBUG("Benchmark '{}' started", m_name);
    }
    
    ~BenchmarkLogger() {
        auto end = Clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - m_start);
        LOG_INFO("Benchmark '{}' completed in {:.3f}ms", 
                 m_name, duration.count() / 1000.0);
    }
};

#define BENCHMARK(name) goldcoin::BenchmarkLogger _bench_##__LINE__(name)

} // namespace goldcoin

#endif // GOLDCOIN_UTIL_LOG_H