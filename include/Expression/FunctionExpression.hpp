#pragma once

#include "Expression.hpp"
#include <functional>
#include <memory>

namespace memesql {

class FunctionExpression : public Expression {
    using Function = std::function<Int(const Cell&)>;

  public:
    FunctionExpression(std::shared_ptr<Expression> expression, Function function)
        : m_function(function),
          m_expression(std::move(expression)) {
    }

    Cell evaluate(const Record& record) const override {
        return m_function(m_expression->evaluate(record));
    }

  private:
    Function m_function;
    std::shared_ptr<Expression> m_expression;
};
} // namespace memesql