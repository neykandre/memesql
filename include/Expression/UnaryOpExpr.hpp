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

    Cell evaluate(const Record& record) const override {
        switch (m_op) {
        case UnaryOpType::NOT:
            return !m_expression->evaluate(record);
        case UnaryOpType::MINUS:
            return -m_expression->evaluate(record);
        case UnaryOpType::LENGTH:
            return m_expression->evaluate(record).length();
        }
    }

    UnaryOpType get_op() const {
        return m_op;
    }

  private:
    UnaryOpType m_op;
    std::shared_ptr<Expression> m_expression;
};
} // namespace memesql