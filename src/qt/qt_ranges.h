// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GOLDCOIN_QT_RANGES_H
#define GOLDCOIN_QT_RANGES_H

//! C++23 Ranges Support for Qt Containers
//! Seamless integration between Qt and std::ranges

#include <ranges>
#include <algorithm>
#include <execution>
#include <span>
#include <QList>
#include <QVector>
#include <QMap>
#include <QHash>
#include <QString>
#include <QStringList>

namespace goldcoin::qt::ranges {

// Concept for Qt container types
template<typename T>
concept QtContainer = requires(T t) {
    typename T::value_type;
    { t.begin() } -> std::input_or_output_iterator;
    { t.end() } -> std::input_or_output_iterator;
    { t.size() } -> std::convertible_to<std::size_t>;
};

// Convert Qt containers to std::span for zero-copy operations
template<typename T>
[[nodiscard]] inline auto as_span(QList<T>& list) noexcept {
    return std::span{list.data(), static_cast<std::size_t>(list.size())};
}

template<typename T>
[[nodiscard]] inline auto as_span(const QList<T>& list) noexcept {
    return std::span<const T>{list.data(), static_cast<std::size_t>(list.size())};
}

// Range adaptor for Qt containers
template<QtContainer Container>
class qt_range_adaptor {
    Container& container;
    
public:
    explicit qt_range_adaptor(Container& c) : container(c) {}
    
    // Filter elements
    template<typename Pred>
    [[nodiscard]] auto filter(Pred&& pred) const {
        return container | std::views::filter(std::forward<Pred>(pred));
    }
    
    // Transform elements
    template<typename Func>
    [[nodiscard]] auto transform(Func&& func) const {
        return container | std::views::transform(std::forward<Func>(func));
    }
    
    // Take first n elements
    [[nodiscard]] auto take(std::size_t n) const {
        return container | std::views::take(n);
    }
    
    // Drop first n elements
    [[nodiscard]] auto drop(std::size_t n) const {
        return container | std::views::drop(n);
    }
    
    // Reverse view
    [[nodiscard]] auto reverse() const {
        return container | std::views::reverse;
    }
};

// Factory function for range adaptor
template<QtContainer Container>
[[nodiscard]] inline auto make_range(Container& c) {
    return qt_range_adaptor{c};
}

// Parallel algorithms for Qt containers
namespace parallel {

template<QtContainer Container, typename Func>
void for_each(Container& container, Func&& func) {
    std::for_each(std::execution::par_unseq,
                  container.begin(), container.end(),
                  std::forward<Func>(func));
}

template<QtContainer Container>
void sort(Container& container) {
    std::sort(std::execution::par_unseq,
              container.begin(), container.end());
}

template<QtContainer Container, typename Comp>
void sort(Container& container, Comp&& comp) {
    std::sort(std::execution::par_unseq,
              container.begin(), container.end(),
              std::forward<Comp>(comp));
}

template<QtContainer Container, typename T>
[[nodiscard]] auto find(const Container& container, const T& value) {
    return std::find(std::execution::par_unseq,
                     container.begin(), container.end(),
                     value);
}

template<QtContainer Container, typename Pred>
[[nodiscard]] auto find_if(const Container& container, Pred&& pred) {
    return std::find_if(std::execution::par_unseq,
                        container.begin(), container.end(),
                        std::forward<Pred>(pred));
}

template<QtContainer Container, typename Pred>
[[nodiscard]] auto count_if(const Container& container, Pred&& pred) {
    return std::count_if(std::execution::par_unseq,
                         container.begin(), container.end(),
                         std::forward<Pred>(pred));
}

} // namespace parallel

// Pipeline operations for Qt containers
template<typename Container>
class pipeline {
    Container data;
    
public:
    explicit pipeline(Container c) : data(std::move(c)) {}
    
    // Chain filter operation
    template<typename Pred>
    [[nodiscard]] auto filter(Pred&& pred) && {
        Container result;
        std::ranges::copy_if(data, std::back_inserter(result),
                             std::forward<Pred>(pred));
        return pipeline{std::move(result)};
    }
    
    // Chain transform operation
    template<typename Func>
    [[nodiscard]] auto transform(Func&& func) && {
        using result_type = decltype(func(data.front()));
        QList<result_type> result;
        std::ranges::transform(data, std::back_inserter(result),
                               std::forward<Func>(func));
        return pipeline{std::move(result)};
    }
    
    // Sort the pipeline
    [[nodiscard]] auto sort() && {
        std::ranges::sort(data);
        return pipeline{std::move(data)};
    }
    
    // Get unique elements
    [[nodiscard]] auto unique() && {
        std::ranges::sort(data);
        auto [first, last] = std::ranges::unique(data);
        data.erase(first, last);
        return pipeline{std::move(data)};
    }
    
    // Terminal operation - collect results
    [[nodiscard]] Container collect() && {
        return std::move(data);
    }
    
    // Terminal operation - get first element
    [[nodiscard]] auto first() const {
        return data.empty() ? std::nullopt : std::optional{data.front()};
    }
    
    // Terminal operation - get last element
    [[nodiscard]] auto last() const {
        return data.empty() ? std::nullopt : std::optional{data.back()};
    }
    
    // Terminal operation - count elements
    [[nodiscard]] std::size_t count() const {
        return data.size();
    }
};

// Factory function for pipeline
template<QtContainer Container>
[[nodiscard]] inline auto from(Container c) {
    return pipeline{std::move(c)};
}

// Specialized operations for QString
namespace string_ops {

[[nodiscard]] inline auto split_view(const QString& str, QChar separator) {
    return str.split(separator) | std::views::transform([](const QString& s) {
        return std::string_view{s.toStdString()};
    });
}

[[nodiscard]] inline auto chars(const QString& str) {
    return std::views::iota(0, str.size()) | std::views::transform([&str](int i) {
        return str[i];
    });
}

} // namespace string_ops

// C++23 zip operations for Qt containers
template<typename Container1, typename Container2>
[[nodiscard]] auto zip(const Container1& c1, const Container2& c2) {
    return std::views::zip(c1, c2);
}

// C++23 enumerate for Qt containers  
template<QtContainer Container>
[[nodiscard]] auto enumerate(Container& c) {
    return std::views::enumerate(c);
}

// Projection support for Qt containers
template<typename Container, typename Proj>
[[nodiscard]] auto project(Container& c, Proj&& proj) {
    return c | std::views::transform(std::forward<Proj>(proj));
}

} // namespace goldcoin::qt::ranges

// Convenience namespace alias
namespace gqtr = goldcoin::qt::ranges;

#endif // GOLDCOIN_QT_RANGES_H