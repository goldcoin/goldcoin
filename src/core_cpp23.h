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
#include <print>
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
template<typename... Args>
inline void LogPrint(std::string_view category, std::string_view fmt, Args&&... args) {
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
        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
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
    
    // Destructor joins all threads
    ~ThreadGroup() {
        interrupt_all();
        join_all();
    }
};

// Modern signal replacement (replaces boost::signals2)
template<typename Signature>
class Signal;

template<typename Return, typename... Args>
class Signal<Return(Args...)> {
private:
    std::vector<std::function<Return(Args...)>> slots;
    mutable std::mutex slots_mutex;
    
public:
    using slot_type = std::function<Return(Args...)>;
    using connection_id = size_t;
    
    // Connect a slot and return connection ID
    connection_id connect(slot_type slot) {
        std::lock_guard<std::mutex> lock(slots_mutex);
        slots.push_back(std::move(slot));
        return slots.size() - 1;
    }
    
    // Disconnect all slots
    void disconnect_all() {
        std::lock_guard<std::mutex> lock(slots_mutex);
        slots.clear();
    }
    
    // Emit signal to all connected slots
    void operator()(Args... args) const {
        std::lock_guard<std::mutex> lock(slots_mutex);
        for (const auto& slot : slots) {
            if (slot) {
                slot(args...);
            }
        }
    }
    
    // Check if any slots are connected
    [[nodiscard]] bool empty() const {
        std::lock_guard<std::mutex> lock(slots_mutex);
        return slots.empty();
    }
    
    // Get number of connected slots
    [[nodiscard]] size_t size() const {
        std::lock_guard<std::mutex> lock(slots_mutex);
        return slots.size();
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
class ChainBuilder {
public:
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
};

} // namespace goldcoin::core

// Legacy compatibility aliases for boost replacements
using thread_group = goldcoin::core::ThreadGroup;

template<typename Signature>
using signal = goldcoin::core::Signal<Signature>;

// Convenience aliases
namespace gc = goldcoin::core;

#endif // GOLDCOIN_CORE_CPP23_H