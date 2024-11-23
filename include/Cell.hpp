#pragma once

#include "Definitions.hpp"
#include <compare>
#include <concepts>
#include <ostream>
#include <type_traits>
#include <variant>

namespace memesql {

using Cell_value = std::variant<Null, Int, Bool, String, Bytes>;

template <typename T>
concept Cell_t = std::is_convertible_v<T, Cell_value>;
    // std::is_same_v<T, Null> || std::is_same_v<T, Int> || std::is_same_v<T, Bool> ||
    // std::is_same_v<T, Bytes> || std::convertible_to<T, String>;

class Cell {
    Cell_value m_value;

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

    bool is_null() const;

    friend std::ostream& operator<<(std::ostream&, const Cell&);

    Cell operator+(const Cell& other) const;
    Cell operator-(const Cell& other) const;
    Cell operator*(const Cell& other) const;
    Cell operator/(const Cell& other) const;
    Cell operator%(const Cell& other) const;

    Cell operator-() const;

    std::strong_ordering operator<=>(const Cell& other) const;
    bool operator==(const Cell& other) const;

    Cell operator&&(const Cell& other) const;
    Cell operator||(const Cell& other) const;
    Cell operator^(const Cell& other) const;
    Cell operator!() const;

    Cell length() const;
    std::string to_string() const;
};
} // namespace memesql