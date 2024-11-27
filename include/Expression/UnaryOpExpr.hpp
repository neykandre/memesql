#pragma once

#include "Expression.hpp"
#include <memory>

namespace memesql {
class UnaryOpExpression : public Expression {
  public:
    UnaryOpExpression(UnaryOpType op, std::shared_ptr<Expression> expression)
        : m_op(op),
          m_expression(expression) {
    }

    Cell evaluate(std::shared_ptr<Table> table, size_t row) const override {
        switch (m_op) {
        case UnaryOpType::NOT:
            return !m_expression->evaluate(table, row);
        case UnaryOpType::MINUS:
            return -m_expression->evaluate(table, row);
        case UnaryOpType::LENGTH:
            return m_expression->evaluate(table, row).length();
        }
        throw DBException("Invalid unary operator");
    }

    UnaryOpType get_op() const {
        return m_op;
    }

  private:
    UnaryOpType m_op;
    std::shared_ptr<Expression> m_expression;
};
} // namespace memesql