#pragma once

#include "Expression.hpp"
#include "Table.hpp"
#include <memory>

namespace memesql {
class UnaryOpExpression : public Expression {
  public:
    UnaryOpExpression(UnaryOpType op, std::shared_ptr<Expression> expression)
        : m_op(op),
          m_expression(expression) {
    }

    Cell evaluate(std::shared_ptr<Table> table,
                  Table::RecordList::const_iterator record_it) const override {
        switch (m_op) {
        case UnaryOpType::NOT:
            return !m_expression->evaluate(table, record_it);
        case UnaryOpType::MINUS:
            return -m_expression->evaluate(table, record_it);
        case UnaryOpType::LENGTH:
            return m_expression->evaluate(table, record_it).length();
        }
        throw DBException("Invalid unary operator");
    }

    UnaryOpType get_op() const {
        return m_op;
    }

    bool is_record_depends() const override {
        return m_expression->is_record_depends();
    }

  private:
    UnaryOpType m_op;
    std::shared_ptr<Expression> m_expression;
};
} // namespace memesql