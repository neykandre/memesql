#pragma once

#include "Definitions.hpp"
#include <compare>
#include <concepts>
#include <ostream>
#include <variant>

namespace memesql {

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

    bool is_null() const;

    friend std::ostream& operator<<(std::ostream&, const Cell&);

    Cell operator+(const Cell& other) const;
    Cell operator-(const Cell& other) const;
    Cell operator*(const Cell& other) const;
    Cell operator/(const Cell& other) const;
    Cell operator%(const Cell& other) const;

    std::strong_ordering operator<=>(const Cell& other) const;
    bool operator==(const Cell& other) const;

    Cell operator&&(const Cell& other) const;
    Cell operator||(const Cell& other) const;
    Cell operator^(const Cell& other) const;
    Cell operator!() const;

};
} // namespace memesql