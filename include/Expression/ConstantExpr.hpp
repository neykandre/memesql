#pragma once

#include "Expression.hpp"

namespace memesql {
class ConstantExpr : public Expression {
  public:
    ConstantExpr() = default;
    explicit ConstantExpr(Cell value)
        : m_value(value) {
    }
    Cell evaluate(std::shared_ptr<Table>,
                  Table::RecordList::const_iterator) const override {
        return m_value;
    }

    bool is_constant() const override {
        return true;
    }

    bool is_record_depends() const override {
        return false;
    }

    const Cell& get_value() const {
        return m_value;
    }

  private:
    Cell m_value;
};
} // namespace memesql