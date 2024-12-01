#pragma once

#include "../Definitions.hpp"
#include "../Exceptions.hpp"
#include "Expression.hpp"
#include "Table.hpp"
#include <memory>

namespace memesql::internal {
class BinaryOpExpr : public Expression {

  public:
    BinaryOpExpr(BinaryOpType op, std::shared_ptr<Expression> left,
                 std::shared_ptr<Expression> right)
        : m_left(std::move(left)),
          m_right(std::move(right)),
          m_op(op) {
    }

    Cell evaluate(std::shared_ptr<Table> table, Table::RecordList::const_iterator record_it) const override {
        switch (m_op) {
        case BinaryOpType::PLUS:
            return m_left->evaluate(table, record_it) + m_right->evaluate(table, record_it);
        case BinaryOpType::MINUS:
            return m_left->evaluate(table, record_it) - m_right->evaluate(table, record_it);
        case BinaryOpType::MUL:
            return m_left->evaluate(table, record_it) * m_right->evaluate(table, record_it);
        case BinaryOpType::DIV:
            return m_left->evaluate(table, record_it) / m_right->evaluate(table, record_it);
        case BinaryOpType::MOD:
            return m_left->evaluate(table, record_it) % m_right->evaluate(table, record_it);
        case BinaryOpType::AND:
            return m_left->evaluate(table, record_it) && m_right->evaluate(table, record_it);
        case BinaryOpType::OR:
            return m_left->evaluate(table, record_it) || m_right->evaluate(table, record_it);
        case BinaryOpType::XOR:
            return m_left->evaluate(table, record_it) ^ m_right->evaluate(table, record_it);
        case BinaryOpType::EQUAL:
            return m_left->evaluate(table, record_it) == m_right->evaluate(table, record_it);
        case BinaryOpType::NOT_EQUAL:
            return m_left->evaluate(table, record_it) != m_right->evaluate(table, record_it);
        case BinaryOpType::GREATER:
            return m_left->evaluate(table, record_it) > m_right->evaluate(table, record_it);
        case BinaryOpType::LESS:
            return m_left->evaluate(table, record_it) < m_right->evaluate(table, record_it);
        case BinaryOpType::GREATER_EQUAL:
            return m_left->evaluate(table, record_it) >= m_right->evaluate(table, record_it);
        case BinaryOpType::LESS_EQUAL:
            return m_left->evaluate(table, record_it) <= m_right->evaluate(table, record_it);
        default:
            throw ExpressionException(
                "unknown binary operation",
                m_left->evaluate(table, record_it).to_string() + " ? " +
                    m_right->evaluate(table, record_it).to_string(),
                m_left->evaluate(table, record_it).to_string().size() + 1);
        }
    }

    bool is_arithmetic() const {
        return m_op > BinaryOpType::ARITHM_START && m_op < BinaryOpType::ARITHM_END;
    }

    bool is_comparison() const {
        return m_op > BinaryOpType::COMPARE_START &&
               m_op < BinaryOpType::COMPARE_END;
    }

    bool is_logical() const {
        return m_op > BinaryOpType::LOGIC_START && m_op < BinaryOpType::LOGIC_END;
    }

    BinaryOpType get_op() const {
        return m_op;
    }

    auto get_left() const {
        return m_left;
    }

    auto get_right() const {
        return m_right;
    }

    bool is_record_depends() const override {
        return m_left->is_record_depends() && m_right->is_record_depends();
    }

  private:
    std::shared_ptr<Expression> m_left;
    std::shared_ptr<Expression> m_right;
    BinaryOpType m_op;
};
} // namespace memesql