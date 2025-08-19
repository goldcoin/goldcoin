// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GOLDCOIN_MEMORY_CPP23_H
#define GOLDCOIN_MEMORY_CPP23_H

//! C++23 Smart Pointer Infrastructure
//! Modern memory management with RAII and zero overhead

#include <memory>
#include <atomic>
#include <concepts>
#include <utility>
#include <type_traits>

namespace goldcoin::memory {

// Concepts for smart pointer compatibility
template<typename T>
concept Deletable = requires(T* ptr) {
    delete ptr;
};

template<typename T>
concept RefCountable = requires(T t) {
    { t.AddRef() };
    { t.Release() };
};

// Custom deleters for special resource types
struct FileDeleter {
    void operator()(FILE* f) const {
        if (f) fclose(f);
    }
};

struct MallocDeleter {
    void operator()(void* p) const {
        if (p) free(p);
    }
};

// Type aliases for common smart pointers
template<typename T>
using UniquePtr = std::unique_ptr<T>;

template<typename T>
using SharedPtr = std::shared_ptr<T>;

template<typename T>
using WeakPtr = std::weak_ptr<T>;

template<typename T>
using AtomicSharedPtr = std::atomic<std::shared_ptr<T>>;

// File handle smart pointer
using UniqueFile = std::unique_ptr<FILE, FileDeleter>;

// Malloc'd memory smart pointer
template<typename T>
using UniqueMalloc = std::unique_ptr<T, MallocDeleter>;

// Factory functions with perfect forwarding
template<typename T, typename... Args>
requires (!std::is_array_v<T>)
[[nodiscard]] inline auto MakeUnique(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
requires std::is_array_v<T>
[[nodiscard]] inline auto MakeUnique(size_t size) {
    return std::make_unique<T>(size);
}

template<typename T, typename... Args>
[[nodiscard]] inline auto MakeShared(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

// Observer pointer (non-owning)
template<typename T>
class ObserverPtr {
    T* ptr = nullptr;
    
public:
    constexpr ObserverPtr() = default;
    constexpr explicit ObserverPtr(T* p) : ptr(p) {}
    constexpr ObserverPtr(std::nullptr_t) : ptr(nullptr) {}
    
    // Construct from smart pointers
    template<typename U>
    ObserverPtr(const std::unique_ptr<U>& p) : ptr(p.get()) {}
    
    template<typename U>
    ObserverPtr(const std::shared_ptr<U>& p) : ptr(p.get()) {}
    
    [[nodiscard]] constexpr T* get() const noexcept { return ptr; }
    [[nodiscard]] constexpr T& operator*() const { return *ptr; }
    [[nodiscard]] constexpr T* operator->() const noexcept { return ptr; }
    [[nodiscard]] constexpr explicit operator bool() const noexcept { return ptr != nullptr; }
    
    void reset(T* p = nullptr) noexcept { ptr = p; }
    void swap(ObserverPtr& other) noexcept { std::swap(ptr, other.ptr); }
};

// Intrusive reference counted base class
class RefCounted {
    mutable std::atomic<uint32_t> ref_count{1};
    
public:
    void AddRef() const {
        ref_count.fetch_add(1, std::memory_order_relaxed);
    }
    
    void Release() const {
        if (ref_count.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete this;
        }
    }
    
    [[nodiscard]] uint32_t GetRefCount() const {
        return ref_count.load(std::memory_order_relaxed);
    }
    
protected:
    virtual ~RefCounted() = default;
};

// Intrusive smart pointer for RefCounted objects
template<RefCountable T>
class IntrusivePtr {
    T* ptr = nullptr;
    
    void AddRef() {
        if (ptr) ptr->AddRef();
    }
    
    void Release() {
        if (ptr) ptr->Release();
    }
    
public:
    IntrusivePtr() = default;
    
    explicit IntrusivePtr(T* p) : ptr(p) {
        AddRef();
    }
    
    IntrusivePtr(const IntrusivePtr& other) : ptr(other.ptr) {
        AddRef();
    }
    
    IntrusivePtr(IntrusivePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }
    
    ~IntrusivePtr() {
        Release();
    }
    
    IntrusivePtr& operator=(const IntrusivePtr& other) {
        if (this != &other) {
            Release();
            ptr = other.ptr;
            AddRef();
        }
        return *this;
    }
    
    IntrusivePtr& operator=(IntrusivePtr&& other) noexcept {
        if (this != &other) {
            Release();
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }
    
    [[nodiscard]] T* get() const { return ptr; }
    [[nodiscard]] T& operator*() const { return *ptr; }
    [[nodiscard]] T* operator->() const { return ptr; }
    [[nodiscard]] explicit operator bool() const { return ptr != nullptr; }
    
    void reset(T* p = nullptr) {
        Release();
        ptr = p;
        AddRef();
    }
    
    T* release() {
        T* tmp = ptr;
        ptr = nullptr;
        return tmp;
    }
};

// RAII lock guard with C++23 features
template<typename Mutex>
class [[nodiscard]] LockGuard {
    Mutex& mutex;
    
public:
    explicit LockGuard(Mutex& m) : mutex(m) {
        mutex.lock();
    }
    
    ~LockGuard() {
        mutex.unlock();
    }
    
    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
};

// Deduction guide
template<typename Mutex>
LockGuard(Mutex&) -> LockGuard<Mutex>;

// C++23 scope guard for cleanup
template<typename Func>
class [[nodiscard]] ScopeGuard {
    Func func;
    bool active = true;
    
public:
    explicit ScopeGuard(Func f) : func(std::move(f)) {}
    
    ~ScopeGuard() {
        if (active) func();
    }
    
    ScopeGuard(ScopeGuard&& other) noexcept
        : func(std::move(other.func)), active(other.active) {
        other.active = false;
    }
    
    void dismiss() { active = false; }
    
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
    ScopeGuard& operator=(ScopeGuard&&) = delete;
};

// Deduction guide
template<typename Func>
ScopeGuard(Func) -> ScopeGuard<Func>;

// Helper to create scope guard
template<typename Func>
[[nodiscard]] auto MakeScopeGuard(Func&& f) {
    return ScopeGuard{std::forward<Func>(f)};
}

// Macro for automatic cleanup
#define SCOPE_EXIT auto UNIQUE_NAME(scope_guard) = goldcoin::memory::MakeScopeGuard
#define UNIQUE_NAME(base) CONCATENATE(base, __LINE__)
#define CONCATENATE(x, y) x##y

} // namespace goldcoin::memory

// Convenience aliases
namespace gm = goldcoin::memory;

#endif // GOLDCOIN_MEMORY_CPP23_H