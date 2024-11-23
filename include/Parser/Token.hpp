#pragma once

#include "../Cell.hpp"
#include <cstddef>
#include <string>
#include <variant>

namespace memesql {

class Token {
  public:
    enum class Type {
#define ALL
#include "TokenEnum.hpp"
#undef ALL
    };

    Token(Type type, Cell_value value, size_t pos)
        : m_type(type),
          m_value(value),
          m_pos(pos) {};

    Token(Type type, size_t pos)
        : m_type(type),
          m_pos(pos) {};

    Type get_type() const {
        return m_type;
    }

    template<Cell_t T>
    T get_value() const {
        return std::get<T>(m_value);
    }

    size_t get_pos() const {
        return m_pos;
    }

    bool is_keyword() const {
        return m_type > Type::KEYWORDS_START && m_type < Type::KEYWORDS_END;
    }

    bool is_literal() const {
        return m_type > Type::LITERALS_START && m_type < Type::LITERALS_END;
    }

    bool is_operator() const {
        return m_type > Type::OPERATORS_START && m_type < Type::OPERATORS_END;
    }

    bool is_punctuation() const {
        return m_type > Type::PUNCTUATION_START && m_type < Type::PUNCTUATION_END;
    }

  private:
    Type m_type;
    Cell_value m_value;
    size_t m_pos;
};
} // namespace memesql