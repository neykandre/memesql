#pragma once

#include "../Definitions.hpp"
#include "Expression.hpp"
#include <memory>

namespace memesql {
class BinOpExpression : public Expression {

  public:
    BinOpExpression(ArithmeticOperator op, std::shared_ptr<Expression> left,
                    std::shared_ptr<Expression> right)
        : m_left(std::move(left)),
          m_right(std::move(right)),
          m_op(op) {
    }

    Cell evaluate(const Record& record) const override {
        switch (m_op) {
        case ArithmeticOperator::ADD:
            return m_left->evaluate(record) + m_right->evaluate(record);
        case ArithmeticOperator::SUB:
            return m_left->evaluate(record) - m_right->evaluate(record);
        case ArithmeticOperator::MUL:
            return m_left->evaluate(record) * m_right->evaluate(record);
        case ArithmeticOperator::DIV:
            return m_left->evaluate(record) / m_right->evaluate(record);
        case ArithmeticOperator::MOD:
            return m_left->evaluate(record) % m_right->evaluate(record);
        }
    }

  private:
    std::shared_ptr<Expression> m_left;
    std::shared_ptr<Expression> m_right;
    ArithmeticOperator m_op;
};
} // namespace memesql