#include "Cell.hpp"
#include "Definitions.hpp"
#include <compare>
#include <cstring>
#include <variant>

namespace memesql {

std::ostream& operator<<(std::ostream& os, const Cell& cell) {
    std::visit(
        [&os](auto&& value) {
            os << value;
        },
        cell.m_value);
    return os;
}
bool Cell::is_null() const {
    return std::holds_alternative<Null>(m_value);
}

Cell Cell::operator+(const Cell& other) const {
    if (std::holds_alternative<Int>(m_value) &&
        std::holds_alternative<Int>(other.m_value)) {
        return std::get<Int>(m_value) + std::get<Int>(other.m_value);
    }
    if (std::holds_alternative<String>(m_value) &&
        std::holds_alternative<String>(other.m_value)) {
        return std::get<String>(m_value) + std::get<String>(other.m_value);
    }
    throw std::invalid_argument("Invalid operation");
}

Cell Cell::operator-(const Cell& other) const {
    if (std::holds_alternative<Int>(m_value) &&
        std::holds_alternative<Int>(other.m_value)) {
        return std::get<Int>(m_value) - std::get<Int>(other.m_value);
    }
    throw std::invalid_argument("Invalid operation");
}

Cell Cell::operator*(const Cell& other) const {
    if (std::holds_alternative<Int>(m_value) &&
        std::holds_alternative<Int>(other.m_value)) {
        return std::get<Int>(m_value) * std::get<Int>(other.m_value);
    }
    throw std::invalid_argument("Invalid operation");
}

Cell Cell::operator/(const Cell& other) const {
    if (std::holds_alternative<Int>(m_value) &&
        std::holds_alternative<Int>(other.m_value)) {
        return std::get<Int>(m_value) / std::get<Int>(other.m_value);
    }
    throw std::invalid_argument("Invalid operation");
}

Cell Cell::operator%(const Cell& other) const {
    if (std::holds_alternative<Int>(m_value) &&
        std::holds_alternative<Int>(other.m_value)) {
        return std::get<Int>(m_value) % std::get<Int>(other.m_value);
    }
    throw std::invalid_argument("Invalid operation");
}

std::strong_ordering Cell::operator<=>(const Cell& other) const {
    return std::visit(
        [&](auto&& value, auto&& other_value) -> std::strong_ordering {
            if constexpr (std::is_same_v<decltype(value), decltype(other_value)>) {
                return value <=> other_value;
            } else {
                throw std::invalid_argument("Invalid comparison");
            }
        },
        m_value, other.m_value);
}

bool Cell::operator==(const Cell& other) const {
    return (*this <=> other) == std::strong_ordering::equal;
}

Cell Cell::operator&&(const Cell& other) const {
    if (std::holds_alternative<Bool>(m_value) &&
        std::holds_alternative<Bool>(other.m_value)) {
        return std::get<Bool>(m_value) && std::get<Bool>(other.m_value);
    }
    throw std::invalid_argument("Invalid operation");
}

Cell Cell::operator||(const Cell& other) const {
    if (std::holds_alternative<Bool>(m_value) &&
        std::holds_alternative<Bool>(other.m_value)) {
        return std::get<Bool>(m_value) || std::get<Bool>(other.m_value);
    }
    throw std::invalid_argument("Invalid operation");
}

Cell Cell::operator^(const Cell& other) const {
    if (std::holds_alternative<Bool>(m_value) &&
        std::holds_alternative<Bool>(other.m_value)) {
        return std::get<Bool>(m_value) ^ std::get<Bool>(other.m_value);
    }
    throw std::invalid_argument("Invalid operation");
}

Cell Cell::operator!() const {
    if (std::holds_alternative<Bool>(m_value)) {
        return !std::get<Bool>(m_value);
    }
    throw std::invalid_argument("Invalid operation");
}

} // namespace memesql