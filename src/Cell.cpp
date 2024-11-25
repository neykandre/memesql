#include "Cell.hpp"
#include "Definitions.hpp"
#include "Exceptions.hpp"
#include <compare>
#include <cstring>
#include <sstream>
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
    throw ExpressionException("+: Incompatible types");
}

Cell Cell::operator-(const Cell& other) const {
    if (std::holds_alternative<Int>(m_value) &&
        std::holds_alternative<Int>(other.m_value)) {
        return std::get<Int>(m_value) - std::get<Int>(other.m_value);
    }
    throw ExpressionException("-: Incompatible types");
}

Cell Cell::operator*(const Cell& other) const {
    if (std::holds_alternative<Int>(m_value) &&
        std::holds_alternative<Int>(other.m_value)) {
        return std::get<Int>(m_value) * std::get<Int>(other.m_value);
    }
    throw ExpressionException("*: Incompatible types");
}

Cell Cell::operator/(const Cell& other) const {
    if (std::holds_alternative<Int>(m_value) &&
        std::holds_alternative<Int>(other.m_value)) {
        return std::get<Int>(m_value) / std::get<Int>(other.m_value);
    }
    throw ExpressionException("/: Incompatible types");
}

Cell Cell::operator%(const Cell& other) const {
    if (std::holds_alternative<Int>(m_value) &&
        std::holds_alternative<Int>(other.m_value)) {
        return std::get<Int>(m_value) % std::get<Int>(other.m_value);
    }
    throw ExpressionException("%: Incompatible types");
}

Cell Cell::operator-() const {
    if (std::holds_alternative<Int>(m_value)) {
        return -std::get<Int>(m_value);
    }
    throw ExpressionException("unary -: Incompatible type");
}

std::strong_ordering Cell::operator<=>(const Cell& other) const {
    return std::visit(
        [&](auto&& value, auto&& other_value) -> std::strong_ordering {
            if constexpr (std::is_same_v<decltype(value), decltype(other_value)>) {
                return value <=> other_value;
            } else {
                throw ExpressionException("Incomparable types");
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
    throw ExpressionException("&&: Incompatible types");
}

Cell Cell::operator||(const Cell& other) const {
    if (std::holds_alternative<Bool>(m_value) &&
        std::holds_alternative<Bool>(other.m_value)) {
        return std::get<Bool>(m_value) || std::get<Bool>(other.m_value);
    }
    throw ExpressionException("||: Incompatible types");
}

Cell Cell::operator^(const Cell& other) const {
    if (std::holds_alternative<Bool>(m_value) &&
        std::holds_alternative<Bool>(other.m_value)) {
        return std::get<Bool>(m_value) ^ std::get<Bool>(other.m_value);
    }
    throw ExpressionException("^: Incompatible types");
}

Cell Cell::operator!() const {
    if (std::holds_alternative<Bool>(m_value)) {
        return !std::get<Bool>(m_value);
    }
    throw ExpressionException("!: Incompatible types");
}

Cell Cell::length() const {
    return std::visit(
        [](auto&& value) -> Cell {
            if constexpr (std::is_same_v<decltype(value), String> ||
                          std::is_same_v<decltype(value), Bytes>) {

                return value.size();
            } else {
                throw ExpressionException("length(): Incompatible type");
            }
        },
        m_value);
}

std::string Cell::to_string() const {
    std::stringstream s;
    s << *this;
    return s.str();
}

const Cell_value& Cell::get_variant() const {
    return m_value;
}

} // namespace memesql