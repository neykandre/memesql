#pragma once

#include "../Definitions.hpp"
#include "Expression.hpp"
#include <memory>

namespace memesql {
class ComparisonExpression : public Expression {
  public:
    ComparisonExpression(ComparisonOperator op, std::shared_ptr<Expression> lhs,
                         std::shared_ptr<Expression> rhs)
        : m_left(std::move(lhs)),
          m_right(std::move(rhs)),
          m_op(op) {
    }
    Cell evaluate(const Record& record) const override {
        switch (m_op) {
        case ComparisonOperator::EQUAL:
            return m_left->evaluate(record) == m_right->evaluate(record);
        case ComparisonOperator::NOT_EQUAL:
            return m_left->evaluate(record) != m_right->evaluate(record);
        case ComparisonOperator::GREATER:
            return m_left->evaluate(record) > m_right->evaluate(record);
        case ComparisonOperator::LESS:
            return m_left->evaluate(record) < m_right->evaluate(record);
        case ComparisonOperator::GREATER_EQUAL:
            return m_left->evaluate(record) >= m_right->evaluate(record);
        case ComparisonOperator::LESS_EQUAL:
            return m_left->evaluate(record) <= m_right->evaluate(record);
        }
    }

    auto get_left() const {
        return m_left;
    }
    auto get_right() const {
        return m_right;
    }
    auto get_op() const {
        return m_op;
    }

  private:
    std::shared_ptr<Expression> m_left;
    std::shared_ptr<Expression> m_right;
    ComparisonOperator m_op;
};
} // namespace memesql