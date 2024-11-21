#include "Condition.hpp"
#include <bits/stdc++.h>

using namespace memesql;

int main() {
    Cell c1{ 1 };
    Cell c2{ "hello" };
    Cell c3{ true };

    Condition::Comparison c = {
        .left_operand  = { c1 },
        .op            = Condition::Comparison::Operator::EQUAL,
        .right_operand = {std::string{"123"}}
    };

    std::cout << c.right_operand.is_column() << std::endl;
}