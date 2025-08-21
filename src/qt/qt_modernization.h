// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GOLDCOIN_QT_MODERNIZATION_H
#define GOLDCOIN_QT_MODERNIZATION_H

//! Qt 6.9 Modernization Helpers
//! Provides modern C++23 wrappers for Qt functionality

#include <QObject>
#include <QString>
#include <QWidget>
#include <functional>
#include <concepts>
#include <expected>
#include <format>
// #include <print>  // Not supported in MinGW GCC 13

// Compatibility shim for std::print
#include <iostream>
namespace std {
    template<typename... Args>
    void print(const std::format_string<Args...> fmt, Args&&... args) {
        std::cout << std::format(fmt, std::forward<Args>(args)...);
    }
    
    template<typename... Args>
    void println(const std::format_string<Args...> fmt, Args&&... args) {
        std::cout << std::format(fmt, std::forward<Args>(args)...) << '\n';
    }
}

namespace goldcoin::qt {

// C++23 concepts for Qt types
template<typename T>
concept QtSignalEmitter = std::derived_from<T, QObject>;

template<typename T>
concept QtWidget = std::derived_from<T, QWidget>;

// Modern signal connection wrapper using C++23
template<QtSignalEmitter Sender, typename Signal, typename Receiver, typename Slot>
requires std::invocable<Slot>
[[nodiscard]] inline auto connectModern(
    Sender* sender,
    Signal signal,
    Receiver* receiver, 
    Slot&& slot,
    Qt::ConnectionType type = Qt::AutoConnection)
{
    return QObject::connect(sender, signal, receiver, std::forward<Slot>(slot), type);
}

// Simplified connection for lambdas
template<QtSignalEmitter Sender, typename Signal, typename Lambda>
requires std::invocable<Lambda>
[[nodiscard]] inline auto connectLambda(
    Sender* sender,
    Signal signal,
    Lambda&& lambda,
    Qt::ConnectionType type = Qt::AutoConnection)
{
    return QObject::connect(sender, signal, std::forward<Lambda>(lambda), type);
}

// Safe signal emission with C++23 std::expected
template<typename T>
using SignalResult = std::expected<T, QString>;

// Modern property binding helper
template<typename T>
class Property {
private:
    T m_value;
    std::function<void(const T&)> m_onChange;
    
public:
    explicit Property(T initial = {}) : m_value(std::move(initial)) {}
    
    Property& operator=(const T& newValue) {
        if (m_value != newValue) {
            m_value = newValue;
            if (m_onChange) {
                m_onChange(m_value);
            }
        }
        return *this;
    }
    
    operator T() const { return m_value; }
    const T& value() const { return m_value; }
    
    void onChange(std::function<void(const T&)> handler) {
        m_onChange = std::move(handler);
    }
};

// Qt string formatting with std::format
template<typename... Args>
[[nodiscard]] inline QString qformat(std::string_view fmt, Args&&... args) {
    return QString::fromStdString(std::format(fmt, std::forward<Args>(args)...));
}

// Modern Qt logging with std::print
inline void qlog(std::string_view message) {
    std::println("[Qt] {}", message);
}

template<typename... Args>
inline void qlog(std::string_view fmt, Args&&... args) {
    std::println("[Qt] {}", std::format(fmt, std::forward<Args>(args)...));
}

// RAII helper for Qt objects
template<QtWidget T>
class WidgetPtr {
private:
    T* m_widget;
    bool m_owned;
    
public:
    explicit WidgetPtr(T* widget, bool owned = false) 
        : m_widget(widget), m_owned(owned) {}
    
    ~WidgetPtr() {
        if (m_owned && m_widget) {
            m_widget->deleteLater();
        }
    }
    
    WidgetPtr(const WidgetPtr&) = delete;
    WidgetPtr& operator=(const WidgetPtr&) = delete;
    
    WidgetPtr(WidgetPtr&& other) noexcept
        : m_widget(std::exchange(other.m_widget, nullptr))
        , m_owned(std::exchange(other.m_owned, false)) {}
    
    WidgetPtr& operator=(WidgetPtr&& other) noexcept {
        if (this != &other) {
            if (m_owned && m_widget) {
                m_widget->deleteLater();
            }
            m_widget = std::exchange(other.m_widget, nullptr);
            m_owned = std::exchange(other.m_owned, false);
        }
        return *this;
    }
    
    T* operator->() { return m_widget; }
    const T* operator->() const { return m_widget; }
    T& operator*() { return *m_widget; }
    const T& operator*() const { return *m_widget; }
    T* get() { return m_widget; }
    const T* get() const { return m_widget; }
    
    [[nodiscard]] bool valid() const { return m_widget != nullptr; }
    
    T* release() {
        m_owned = false;
        return std::exchange(m_widget, nullptr);
    }
};

// Helper to make signal connections more readable
struct SignalConnector {
    QObject* sender;
    
    template<typename Signal, typename Receiver, typename Slot>
    SignalConnector& connect(Signal signal, Receiver* receiver, Slot&& slot) {
        QObject::connect(sender, signal, receiver, std::forward<Slot>(slot));
        return *this;
    }
    
    template<typename Signal, typename Lambda>
    SignalConnector& connect(Signal signal, Lambda&& lambda) {
        QObject::connect(sender, signal, std::forward<Lambda>(lambda));
        return *this;
    }
};

[[nodiscard]] inline SignalConnector from(QObject* sender) {
    return SignalConnector{sender};
}

} // namespace goldcoin::qt

// Macros for backward compatibility (to be phased out)
#define QT_MODERN_CONNECT(sender, signal, receiver, slot) \
    goldcoin::qt::connectModern(sender, signal, receiver, slot)

#define QT_LAMBDA_CONNECT(sender, signal, lambda) \
    goldcoin::qt::connectLambda(sender, signal, lambda)

#endif // GOLDCOIN_QT_MODERNIZATION_H