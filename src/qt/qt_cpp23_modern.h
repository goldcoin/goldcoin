// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GOLDCOIN_QT_CPP23_MODERN_H
#define GOLDCOIN_QT_CPP23_MODERN_H

//! C++23/Qt 6.9 Modernization Layer
//! Provides modern C++23 features and Qt6.9 optimizations

#include <expected>
#include <span>
#include <ranges>
#include <format>
#include <print>
#include <chrono>
#include <concepts>
#include <coroutine>
#include <memory>
#include <string_view>
#include <variant>
#include <optional>
#include <functional>
#include <algorithm>
#include <execution>

#include <QObject>
#include <QString>
#include <QList>
#include <QVector>
#include <QMap>

namespace goldcoin::qt {

// C++23 Concepts for Qt types
template<typename T>
concept QtObject = std::derived_from<T, QObject>;

template<typename T>
concept QtWidget = QtObject<T> && requires(T t) {
    { t.show() } -> std::same_as<void>;
    { t.hide() } -> std::same_as<void>;
};

// Modern signal/slot connection with C++23
template<QtObject Sender, QtObject Receiver>
class ModernConnection {
public:
    template<typename Signal, typename Slot>
    static auto connect(Sender* sender, Signal signal, Receiver* receiver, Slot slot) {
        if constexpr (std::invocable<Slot>) {
            return QObject::connect(sender, signal, receiver, slot, Qt::AutoConnection);
        } else {
            return QObject::connect(sender, signal, receiver, slot);
        }
    }
};

// C++23 Range adaptor for Qt containers
template<typename Container>
class QtRange {
    Container& container;
public:
    explicit QtRange(Container& c) : container(c) {}
    
    auto begin() { return container.begin(); }
    auto end() { return container.end(); }
    
    // C++23 range operations
    template<typename Pred>
    auto filter(Pred pred) {
        return container | std::views::filter(pred);
    }
    
    template<typename Func>
    auto transform(Func func) {
        return container | std::views::transform(func);
    }
    
    auto take(size_t n) {
        return container | std::views::take(n);
    }
};

// Convert Qt containers to std::span for zero-copy operations
template<typename T>
auto to_span(QList<T>& list) {
    return std::span{list.data(), static_cast<size_t>(list.size())};
}

template<typename T>
auto to_span(const QList<T>& list) {
    return std::span<const T>{list.data(), static_cast<size_t>(list.size())};
}

// C++23 formatted output for Qt types
template<>
struct std::formatter<QString> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }
    
    auto format(const QString& s, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{}", s.toStdString());
    }
};

// Modern error handling with std::expected
template<typename T>
using QtResult = std::expected<T, QString>;

// C++23 Coroutine support for async Qt operations
template<typename T>
struct QtAsync {
    struct promise_type {
        T value;
        
        QtAsync get_return_object() {
            return {std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        
        void return_value(T v) { value = std::move(v); }
        void unhandled_exception() { std::terminate(); }
    };
    
    std::coroutine_handle<promise_type> h;
    
    T get() {
        return h.promise().value;
    }
    
    ~QtAsync() {
        if (h) h.destroy();
    }
};

// Smart pointer wrapper for Qt objects with RAII
template<QtObject T>
class QtSmartPtr {
    std::unique_ptr<T> ptr;
    
public:
    explicit QtSmartPtr(T* obj) : ptr(obj) {
        if (obj) {
            obj->setParent(nullptr); // Ensure Qt doesn't auto-delete
        }
    }
    
    T* get() const { return ptr.get(); }
    T* operator->() const { return ptr.get(); }
    T& operator*() const { return *ptr; }
    
    T* release() { return ptr.release(); }
    void reset(T* obj = nullptr) { 
        if (obj) obj->setParent(nullptr);
        ptr.reset(obj); 
    }
};

// C++23 Pattern matching for Qt events
template<typename... Handlers>
class EventMatcher {
    std::tuple<Handlers...> handlers;
    
public:
    explicit EventMatcher(Handlers... h) : handlers(h...) {}
    
    template<typename Event>
    bool handle(Event* event) {
        return std::apply([event](auto&&... handler) {
            return (try_handle(event, handler) || ...);
        }, handlers);
    }
    
private:
    template<typename Event, typename Handler>
    static bool try_handle(Event* event, Handler& handler) {
        if constexpr (std::invocable<Handler, Event*>) {
            handler(event);
            return true;
        }
        return false;
    }
};

// Parallel execution for Qt operations
template<typename Container, typename Func>
void parallel_foreach(Container& container, Func func) {
    std::for_each(std::execution::par_unseq,
                  container.begin(), container.end(),
                  func);
}

// C++23 string formatting utilities
inline QString format_qt(std::string_view fmt, auto&&... args) {
    return QString::fromStdString(std::format(fmt, std::forward<decltype(args)>(args)...));
}

// Modern logging with std::print
inline void log_qt(std::string_view fmt, auto&&... args) {
    std::print("{}\n", std::format(fmt, std::forward<decltype(args)>(args)...));
}

// Zero-cost abstraction for Qt property binding
template<typename T>
class QtProperty {
    T value;
    std::function<void(const T&)> onChange;
    
public:
    QtProperty() = default;
    explicit QtProperty(T val) : value(std::move(val)) {}
    
    QtProperty& operator=(const T& val) {
        if (value != val) {
            value = val;
            if (onChange) onChange(value);
        }
        return *this;
    }
    
    operator T() const { return value; }
    const T& get() const { return value; }
    
    void bind(std::function<void(const T&)> callback) {
        onChange = std::move(callback);
    }
};

// C++23 Deducing this for Qt method chaining
class QtChainable : public QObject {
public:
    template<typename Self>
    auto&& setProperty(this Self&& self, const char* name, const QVariant& value) {
        self.QObject::setProperty(name, value);
        return std::forward<Self>(self);
    }
    
    template<typename Self>
    auto&& setObjectName(this Self&& self, const QString& name) {
        self.QObject::setObjectName(name);
        return std::forward<Self>(self);
    }
};

} // namespace goldcoin::qt

// Convenience aliases
namespace gqt = goldcoin::qt;

#endif // GOLDCOIN_QT_CPP23_MODERN_H