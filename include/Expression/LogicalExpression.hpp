#pragma once

#include "../Definitions.hpp"
#include "Expression.hpp"
#include <memory>

namespace memesql {

class LogicalExpression : public Expression {
  public:
    LogicalExpression(Logical type, std::shared_ptr<Expression> left,
                      std::shared_ptr<Expression> right)
        : type(type),
          m_left(std::move(left)),
          m_right(std::move(right)) {
    }

    Cell evaluate(const Record& record) const override {
        switch (type) {
        case Logical::AND:
            return m_left->evaluate(record) && m_right->evaluate(record);
        case Logical::OR:
            return m_left->evaluate(record) || m_right->evaluate(record);
        case Logical::XOR:
            return m_left->evaluate(record) ^ m_right->evaluate(record);
        case Logical::NOT:
            return !m_left->evaluate(record);
        }
    }

  private:
    Logical type;
    std::shared_ptr<Expression> m_left;
    std::shared_ptr<Expression> m_right;
};
} // namespace memesql