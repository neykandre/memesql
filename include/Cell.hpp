#pragma once

#include <concepts>
#include <cstdint>
#include <ostream>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace memesql {

using Null   = std::monostate;
using Int    = int32_t;
using Bool   = bool;
using String = std::string;

class Bytes {
    std::vector<uint8_t> m_bytes;

  public:
    Bytes(std::string_view sv);

    friend std::ostream& operator<<(std::ostream&, const Bytes&);
};

template <typename T>
concept Cell_t =
    std::is_same_v<T, Null> || std::is_same_v<T, Int> || std::is_same_v<T, Bool> ||
    std::is_same_v<T, Bytes> || std::convertible_to<T, String>;

class Cell {
    std::variant<Null, Int, Bool, String, Bytes> m_value;

  public:
    Cell() = default;

    template <Cell_t T>
    Cell(T&& value)
        : m_value{ std::forward<T>(value) } {
    }

    template <Cell_t T>
    const T& get() const {
        return std::get<T>(m_value);
    }

    template <Cell_t T>
    void set(T&& value) {
        m_value = value;
    }

    bool is_null() const;

    friend std::ostream& operator<<(std::ostream&, const Cell&);
};
} // namespace memesql