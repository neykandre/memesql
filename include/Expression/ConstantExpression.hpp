#pragma once

#include "Expression.hpp"

namespace memesql {
class ConstantExpression : public Expression {
  public:
    explicit ConstantExpression(Cell value)
        : m_value(value) {
    }
    Cell evaluate(const Record&) const override {
        return m_value;
    }

    bool is_constant() const override {
        return true;
    }

    const Cell& get_value() const {
        return m_value;
    }

  private:
    Cell m_value;
};
} // namespace memesql