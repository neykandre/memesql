#pragma once

#include "../Definitions.hpp"
#include "../Exceptions.hpp"
#include "Expression.hpp"
#include <memory>

namespace memesql {
class BinaryOpExpr : public Expression {

  public:
    BinaryOpExpr(BinaryOpType op, std::shared_ptr<Expression> left,
                    std::shared_ptr<Expression> right)
        : m_left(std::move(left)),
          m_right(std::move(right)),
          m_op(op) {
    }

    Cell evaluate(const Record& record) const override {
        switch (m_op) {
        case BinaryOpType::ADD:
            return m_left->evaluate(record) + m_right->evaluate(record);
        case BinaryOpType::SUB:
            return m_left->evaluate(record) - m_right->evaluate(record);
        case BinaryOpType::MUL:
            return m_left->evaluate(record) * m_right->evaluate(record);
        case BinaryOpType::DIV:
            return m_left->evaluate(record) / m_right->evaluate(record);
        case BinaryOpType::MOD:
            return m_left->evaluate(record) % m_right->evaluate(record);
        case BinaryOpType::AND:
            return m_left->evaluate(record) && m_right->evaluate(record);
        case BinaryOpType::OR:
            return m_left->evaluate(record) || m_right->evaluate(record);
        case BinaryOpType::XOR:
            return m_left->evaluate(record) ^ m_right->evaluate(record);
        case BinaryOpType::EQUAL:
            return m_left->evaluate(record) == m_right->evaluate(record);
        case BinaryOpType::NOT_EQUAL:
            return m_left->evaluate(record) != m_right->evaluate(record);
        case BinaryOpType::GREATER:
            return m_left->evaluate(record) > m_right->evaluate(record);
        case BinaryOpType::LESS:
            return m_left->evaluate(record) < m_right->evaluate(record);
        case BinaryOpType::GREATER_EQUAL:
            return m_left->evaluate(record) >= m_right->evaluate(record);
        case BinaryOpType::LESS_EQUAL:
            return m_left->evaluate(record) <= m_right->evaluate(record);
        default:
            throw ExpressionException("Unknown binary operation");
        }
    }

    bool is_arithmetic() const {
        return m_op > BinaryOpType::ARITHM_START && m_op < BinaryOpType::ARITHM_END;
    }

    bool is_comparison() const {
        return m_op > BinaryOpType::COMPARE_START && m_op < BinaryOpType::COMPARE_END;
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

  private:
    std::shared_ptr<Expression> m_left;
    std::shared_ptr<Expression> m_right;
    BinaryOpType m_op;
};
} // namespace memesql