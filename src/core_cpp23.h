// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GOLDCOIN_CORE_CPP23_H
#define GOLDCOIN_CORE_CPP23_H

//! C++23 Modernization for Core Layer
//! Provides modern C++23 features for core blockchain functionality

#include <expected>
#include <optional>
#include <span>
#include <ranges>
#include <format>
// C++23 print support - GCC 15.1 has full support
// MinGW GCC 13 compatibility fallback for Windows cross-compilation only
#if __has_include(<print>)
#include <print>
#else
// Temporary fallback for MinGW GCC 13 cross-compilation
// TODO: Remove when MinGW updates to GCC 15.1
#include <iostream>
#include <cstdio>
namespace std {
    template<typename... Args>
    void print(std::string_view fmt, Args&&... args) {
        if constexpr (sizeof...(args) == 0) {
            printf("%s", fmt.data());
        } else {
            // MinGW GCC 13 doesn't support format string as template parameter
            // Using vformat as workaround
            printf("%s", vformat(fmt, make_format_args(forward<Args>(args)...)).c_str());
        }
    }
    template<typename... Args>
    void print(FILE* stream, std::string_view fmt, Args&&... args) {
        if constexpr (sizeof...(args) == 0) {
            fprintf(stream, "%s", fmt.data());
        } else {
            fprintf(stream, "%s", vformat(fmt, make_format_args(forward<Args>(args)...)).c_str());
        }
    }
}
#endif
#include <concepts>
#include <memory>
#include <string_view>
#include <filesystem>
#include <chrono>
#include <variant>
#include <coroutine>
#include <thread>
#include <vector>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

// Forward declarations for blockchain types
class uint256;
class CBlock;
class CTransaction;
class CTxMemPoolEntry;
typedef int64_t CAmount;

namespace goldcoin::core {

// C++23 Concepts for blockchain types
template<typename T>
concept Hashable = requires(T t) {
    { t.GetHash() } -> std::convertible_to<uint256>;
};

template<typename T>
concept Transaction = Hashable<T> && requires(T t) {
    { t.GetValueOut() } -> std::convertible_to<CAmount>;
    { t.IsCoinBase() } -> std::convertible_to<bool>;
};

template<typename T>
concept Block = Hashable<T> && requires(T t) {
    { t.vtx } -> std::ranges::range;
    { t.nNonce } -> std::convertible_to<uint32_t>;
};

// Modern error handling with std::expected
enum class ValidationError {
    NONE,
    INVALID_HEADER,
    INVALID_PROOF_OF_WORK,
    TIMESTAMP_TOO_FAR,
    INVALID_MERKLE_ROOT,
    BLOCK_TOO_LARGE,
    TRANSACTION_INVALID,
    DUPLICATE_TRANSACTION,
    INSUFFICIENT_FEE
};

template<typename T>
using ValidationResult = std::expected<T, ValidationError>;

// Smart pointer aliases for RAII
template<typename T>
using UniquePtr = std::unique_ptr<T>;

template<typename T>
using SharedPtr = std::shared_ptr<T>;

template<typename T>
using WeakPtr = std::weak_ptr<T>;

// Make smart pointers with perfect forwarding
template<typename T, typename... Args>
[[nodiscard]] inline auto MakeUnique(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
[[nodiscard]] inline auto MakeShared(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

// C++23 ranges for blockchain data
template<std::ranges::range Container>
class BlockchainRange {
    Container& container;
    
public:
    explicit BlockchainRange(Container& c) : container(c) {}
    
    // Filter valid transactions
    [[nodiscard]] auto valid_txs() const {
        return container | std::views::filter([](const auto& tx) {
            return tx.IsValid();
        });
    }
    
    // Get coinbase transactions
    [[nodiscard]] auto coinbase_txs() const {
        return container | std::views::filter([](const auto& tx) {
            return tx.IsCoinBase();
        });
    }
    
    // Transform to amounts
    [[nodiscard]] auto amounts() const {
        return container | std::views::transform([](const auto& tx) {
            return tx.GetValueOut();
        });
    }
    
    // Calculate total value
    [[nodiscard]] CAmount total_value() const {
        CAmount total = 0;
        for (const auto& amount : amounts()) {
            total += amount;
        }
        return total;
    }
};

// Zero-copy string operations
using string_view = std::string_view;

[[nodiscard]] inline string_view trim(string_view str) {
    const auto first = str.find_first_not_of(" \t\n\r");
    if (first == string_view::npos) return {};
    const auto last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

// Modern filesystem operations (replacing boost::filesystem)
namespace fs = std::filesystem;

[[nodiscard]] inline bool PathExists(const fs::path& path) {
    return fs::exists(path);
}

[[nodiscard]] inline std::optional<std::uintmax_t> GetFileSize(const fs::path& path) {
    std::error_code ec;
    auto size = fs::file_size(path, ec);
    if (ec) return std::nullopt;
    return size;
}

[[nodiscard]] inline bool CreateDirectories(const fs::path& path) {
    std::error_code ec;
    return fs::create_directories(path, ec);
}

// C++23 formatted output
// Renamed to avoid conflict with macro
template<typename... Args>
inline void LogPrintFormatted(std::string_view category, std::string_view fmt, Args&&... args) {
    std::print("[{}] {}\n", category, std::format(fmt, std::forward<Args>(args)...));
}

template<typename... Args>
inline void LogError(std::string_view fmt, Args&&... args) {
    std::print(stderr, "ERROR: {}\n", std::format(fmt, std::forward<Args>(args)...));
}

// Performance timing with C++23 chrono
class ScopedTimer {
    using clock = std::chrono::high_resolution_clock;
    using time_point = clock::time_point;
    
    time_point start;
    std::string_view name;
    
public:
    explicit ScopedTimer(std::string_view n) : start(clock::now()), name(n) {}
    
    ~ScopedTimer() {
        auto end = clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::print("[TIMER] {} took {} μs\n", name, duration.count());
    }
};

// Coroutine support for async blockchain operations
template<typename T>
struct ChainAsync {
    struct promise_type {
        T value;
        
        ChainAsync get_return_object() {
            return {std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        
        void return_value(T v) { value = std::move(v); }
        void unhandled_exception() { std::terminate(); }
    };
    
    std::coroutine_handle<promise_type> h;
    
    [[nodiscard]] T get() {
        return h.promise().value;
    }
    
    ~ChainAsync() {
        if (h) h.destroy();
    }
};

// Pattern matching for chain state
template<typename... Handlers>
class ChainStateMatcher {
    std::tuple<Handlers...> handlers;
    
public:
    explicit ChainStateMatcher(Handlers... h) : handlers(h...) {}
    
    template<typename State>
    auto handle(const State& state) {
        return std::apply([&state](auto&&... handler) {
            return (try_handle(state, handler) || ...);
        }, handlers);
    }
    
private:
    template<typename State, typename Handler>
    static bool try_handle(const State& state, Handler& handler) {
        if constexpr (std::invocable<Handler, State>) {
            handler(state);
            return true;
        }
        return false;
    }
};

// Modern thread management (replaces boost::thread_group)
class ThreadGroup {
private:
    std::vector<std::thread> threads;
    std::atomic<bool> interrupted{false};
    
public:
    ThreadGroup() = default;
    
    // Non-copyable, movable
    ThreadGroup(const ThreadGroup&) = delete;
    ThreadGroup& operator=(const ThreadGroup&) = delete;
    ThreadGroup(ThreadGroup&&) = default;
    ThreadGroup& operator=(ThreadGroup&&) = default;
    
    // Create and add a thread
    template<typename Function, typename... Args>
    void create_thread(Function&& func, Args&&... args) {
        threads.emplace_back(std::forward<Function>(func), std::forward<Args>(args)...);
    }
    
    // Add an existing thread
    void add_thread(std::thread&& thread) {
        threads.push_back(std::move(thread));
    }
    
    // Interrupt all threads (sets flag, actual interruption is implementation-specific)
    void interrupt_all() {
        interrupted = true;
    }
    
    // Check if interrupted
    [[nodiscard]] bool is_interrupted() const {
        return interrupted;
    }
    
    // Join all threads
    void join_all() {
        size_t i = 0;
        for (auto& thread : threads) {
            if (thread.joinable()) {
                // Debug logging - requires util.h to be included by caller
                // LogPrintf("ThreadGroup::join_all: Waiting for thread %zu to join...\n", i);
                thread.join();
                // LogPrintf("ThreadGroup::join_all: Thread %zu joined\n", i);
            } else {
                // LogPrintf("ThreadGroup::join_all: Thread %zu is not joinable (already joined or detached)\n", i);
            }
            i++;
        }
        threads.clear();
    }
    
    // Get size
    [[nodiscard]] size_t size() const {
        return threads.size();
    }
    
    // Check if empty
    [[nodiscard]] bool empty() const {
        return threads.empty();
    }
    
    // Get underlying thread vector (for compatibility)
    std::vector<std::thread>& get_threads() {
        return threads;
    }
    
    // Destructor joins all threads
    ~ThreadGroup() {
        interrupt_all();
        join_all();
    }
};

// Modern signal replacement (replaces boost::signals2)
// MinGW GCC 13 workaround: Use non-specialized template with enable_if
template<typename Signature>
class Signal {
private:
    // Helper to extract function signature
    template<typename T> struct signature_traits;
    
    template<typename Return, typename... Args>
    struct signature_traits<Return(Args...)> {
        using return_type = Return;
        using slot_type = std::function<Return(Args...)>;
        
        template<typename... CallArgs>
        static void call_slot(const slot_type& slot, CallArgs&&... args) {
            if (slot) {
                slot(std::forward<CallArgs>(args)...);
            }
        }
    };
    
    using traits = signature_traits<Signature>;
    using slot_type = typename traits::slot_type;
    
    std::vector<slot_type> slots_;
    mutable std::mutex mutex_;
    
public:
    using connection_id = size_t;
    
    // Connect a slot and return connection ID
    connection_id connect(slot_type slot) {
        std::lock_guard<std::mutex> lock(mutex_);
        slots_.push_back(std::move(slot));
        return slots_.size() - 1;
    }
    
    // Disconnect all slots
    void disconnect_all() {
        std::lock_guard<std::mutex> lock(mutex_);
        slots_.clear();
    }
    
    // Emit signal to all connected slots
    template<typename... Args>
    void operator()(Args&&... args) const {
        std::lock_guard<std::mutex> lock(mutex_);
        for (const auto& slot : slots_) {
            traits::call_slot(slot, std::forward<Args>(args)...);
        }
    }
    
    // Check if any slots are connected
    [[nodiscard]] bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return slots_.empty();
    }
    
    // Get number of connected slots
    [[nodiscard]] size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return slots_.size();
    }
};

// Thread-safe utilities
template<typename T>
class ThreadSafe {
private:
    mutable std::mutex mutex_;
    T data_;
    
public:
    template<typename... Args>
    explicit ThreadSafe(Args&&... args) : data_(std::forward<Args>(args)...) {}
    
    // Execute function with locked access
    template<typename Function>
    auto with_lock(Function&& func) const -> decltype(func(data_)) {
        std::lock_guard<std::mutex> lock(mutex_);
        return func(data_);
    }
    
    // Execute function with locked access (non-const)
    template<typename Function>
    auto with_lock(Function&& func) -> decltype(func(data_)) {
        std::lock_guard<std::mutex> lock(mutex_);
        return func(data_);
    }
    
    // Copy assignment
    ThreadSafe& operator=(const T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        data_ = value;
        return *this;
    }
    
    // Move assignment
    ThreadSafe& operator=(T&& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        data_ = std::move(value);
        return *this;
    }
    
    // Get copy of value
    [[nodiscard]] T get() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return data_;
    }
};

// C++23 deducing this for method chaining
// MinGW GCC 13 compatibility: Using traditional CRTP pattern instead of deducing this
class ChainBuilder {
public:
#if __cpp_explicit_this_parameter >= 202110L
    // GCC 15.1 path - full C++23 deducing this
    template<typename Self>
    auto&& withBlock(this Self&& self, const CBlock& block) {
        // Process block
        return std::forward<Self>(self);
    }
    
    template<typename Self>
    auto&& withTransaction(this Self&& self, const CTransaction& tx) {
        // Process transaction
        return std::forward<Self>(self);
    }
    
    template<typename Self>
    auto&& validate(this Self&& self) {
        // Validate chain
        return std::forward<Self>(self);
    }
#else
    // MinGW GCC 13 fallback - traditional method chaining
    ChainBuilder& withBlock(const CBlock& block) {
        // Process block
        return *this;
    }
    
    ChainBuilder& withTransaction(const CTransaction& tx) {
        // Process transaction
        return *this;
    }
    
    ChainBuilder& validate() {
        // Validate chain
        return *this;
    }
#endif
};

} // namespace goldcoin::core

// Legacy compatibility aliases for boost replacements
using thread_group = goldcoin::core::ThreadGroup;

// Note: Can't use 'signal' as it conflicts with signal.h
template<typename Signature>
using gc_signal = goldcoin::core::Signal<Signature>;

// Convenience aliases
namespace gc = goldcoin::core;

#endif // GOLDCOIN_CORE_CPP23_H