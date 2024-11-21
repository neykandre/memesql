// #include "Condition.hpp"
#include "Cell.hpp"
#include "Definitions.hpp"
#include "Expression/ComparisonExpression.hpp"
#include "Expression/ConstantExpression.hpp"
#include "Expression/ColumnExpression.hpp"
#include "Expression/Expression.hpp"
#include <bits/stdc++.h>
#include <memory>

using namespace memesql;

int main() {
    // Cell c1{ 1 };
    // Cell c2{ "hello" };
    // Cell c3{ true };

    // Condition::Comparison c = {
    //     .left_operand  = { c1 },
    //     .op            = Condition::Comparison::Operator::EQUAL,
    //     .right_operand = {std::string{"123"}}
    // };

    Cell c1{ 2 };
    Cell c2{ 2 };

    auto ce1 = std::make_shared<ConstantExpression>(c1);
    auto ce2 = std::make_shared<ColumnExpression>("test");

    std::shared_ptr<Expression> e1 = ce2;

    std::cout << e1->is_column() << std::endl;
}