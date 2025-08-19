// Copyright (c) 2025 The Goldcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GOLDCOIN_SERIALIZE_CPP23_H
#define GOLDCOIN_SERIALIZE_CPP23_H

//! C++23 Modernization for Serialization
//! Concepts and modern features for type-safe serialization

#include <concepts>
#include <span>
#include <ranges>
#include <expected>
#include <format>
#include <bit>
#include <cstdint>
#include <type_traits>

namespace goldcoin::serialize {

// C++23 Concepts for serializable types
template<typename T>
concept Integral = std::integral<T>;

template<typename T>
concept UnsignedIntegral = std::unsigned_integral<T>;

template<typename T>
concept SignedIntegral = std::signed_integral<T>;

template<typename T>
concept Serializable = requires(T t, std::span<uint8_t> buffer) {
    { t.Serialize(buffer) } -> std::same_as<size_t>;
    { T::Deserialize(buffer) } -> std::same_as<std::expected<T, std::string>>;
};

template<typename T>
concept HasGetSerializeSize = requires(T t) {
    { t.GetSerializeSize() } -> std::convertible_to<size_t>;
};

template<typename Stream>
concept OutputStream = requires(Stream s, const uint8_t* data, size_t size) {
    { s.write(data, size) };
    { s.flush() };
};

template<typename Stream>
concept InputStream = requires(Stream s, uint8_t* data, size_t size) {
    { s.read(data, size) } -> std::convertible_to<size_t>;
    { s.eof() } -> std::convertible_to<bool>;
};

// Modern serialization with C++23 features
template<UnsignedIntegral T>
[[nodiscard]] constexpr auto to_little_endian(T value) {
    if constexpr (std::endian::native == std::endian::little) {
        return value;
    } else {
        return std::byteswap(value);
    }
}

template<UnsignedIntegral T>
[[nodiscard]] constexpr auto from_little_endian(T value) {
    return to_little_endian(value); // Same operation for both directions
}

// Variable-length integer encoding (CompactSize)
template<UnsignedIntegral T>
[[nodiscard]] constexpr size_t GetCompactSizeLength(T value) {
    if (value < 253) return 1;
    else if (value <= std::numeric_limits<uint16_t>::max()) return 3;
    else if (value <= std::numeric_limits<uint32_t>::max()) return 5;
    else return 9;
}

// Serialize to span with bounds checking
template<UnsignedIntegral T>
[[nodiscard]] std::expected<size_t, std::string> WriteCompactSize(std::span<uint8_t> buffer, T value) {
    const size_t needed = GetCompactSizeLength(value);
    if (buffer.size() < needed) {
        return std::unexpected(std::format("Buffer too small: {} < {}", buffer.size(), needed));
    }
    
    if (value < 253) {
        buffer[0] = static_cast<uint8_t>(value);
        return 1;
    } else if (value <= std::numeric_limits<uint16_t>::max()) {
        buffer[0] = 253;
        auto le_value = to_little_endian(static_cast<uint16_t>(value));
        std::memcpy(&buffer[1], &le_value, 2);
        return 3;
    } else if (value <= std::numeric_limits<uint32_t>::max()) {
        buffer[0] = 254;
        auto le_value = to_little_endian(static_cast<uint32_t>(value));
        std::memcpy(&buffer[1], &le_value, 4);
        return 5;
    } else {
        buffer[0] = 255;
        auto le_value = to_little_endian(static_cast<uint64_t>(value));
        std::memcpy(&buffer[1], &le_value, 8);
        return 9;
    }
}

// Read CompactSize with validation
[[nodiscard]] inline std::expected<std::pair<uint64_t, size_t>, std::string> 
ReadCompactSize(std::span<const uint8_t> buffer) {
    if (buffer.empty()) {
        return std::unexpected("Buffer is empty");
    }
    
    uint8_t first = buffer[0];
    if (first < 253) {
        return std::make_pair(first, 1);
    } else if (first == 253) {
        if (buffer.size() < 3) {
            return std::unexpected("Buffer too small for uint16");
        }
        uint16_t value;
        std::memcpy(&value, &buffer[1], 2);
        return std::make_pair(from_little_endian(value), 3);
    } else if (first == 254) {
        if (buffer.size() < 5) {
            return std::unexpected("Buffer too small for uint32");
        }
        uint32_t value;
        std::memcpy(&value, &buffer[1], 4);
        return std::make_pair(from_little_endian(value), 5);
    } else {
        if (buffer.size() < 9) {
            return std::unexpected("Buffer too small for uint64");
        }
        uint64_t value;
        std::memcpy(&value, &buffer[1], 8);
        return std::make_pair(from_little_endian(value), 9);
    }
}

// Range-based serialization for containers
template<std::ranges::range Container>
requires Serializable<typename Container::value_type>
[[nodiscard]] size_t GetSerializeSize(const Container& container) {
    size_t size = GetCompactSizeLength(container.size());
    for (const auto& item : container) {
        size += item.GetSerializeSize();
    }
    return size;
}

// Serialize container with ranges
template<std::ranges::range Container, OutputStream Stream>
requires Serializable<typename Container::value_type>
void Serialize(Stream& s, const Container& container) {
    WriteCompactSize(s, container.size());
    std::ranges::for_each(container, [&s](const auto& item) {
        item.Serialize(s);
    });
}

// Type-safe wrapper for serialized data
template<typename T>
class SerializedData {
    std::vector<uint8_t> data;
    
public:
    explicit SerializedData(const T& obj) requires Serializable<T> {
        data.reserve(obj.GetSerializeSize());
        std::span<uint8_t> buffer{data};
        obj.Serialize(buffer);
    }
    
    [[nodiscard]] std::span<const uint8_t> GetData() const { return data; }
    [[nodiscard]] size_t GetSize() const { return data.size(); }
    
    [[nodiscard]] std::expected<T, std::string> Deserialize() const {
        std::span<const uint8_t> buffer{data};
        return T::Deserialize(buffer);
    }
};

// C++23 deducing this for method chaining
class SerializeBuilder {
public:
    template<typename Self, Serializable T>
    auto&& add(this Self&& self, const T& obj) {
        // Add object to serialization
        return std::forward<Self>(self);
    }
    
    template<typename Self>
    [[nodiscard]] auto build(this Self&& self) {
        // Build final serialized result
        return std::forward<Self>(self);
    }
};

} // namespace goldcoin::serialize

#endif // GOLDCOIN_SERIALIZE_CPP23_H