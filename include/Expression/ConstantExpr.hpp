#pragma once

#include "Expression.hpp"

namespace memesql {
class ConstantExpr : public Expression {
  public:
    explicit ConstantExpr(Cell value)
        : m_value(value) {
    }
    Cell evaluate(const Table&, size_t) const override {
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