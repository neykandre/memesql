#include "../include/Expression/BinOpExpression.hpp"
#include "../include/Record.hpp"
#include "gtest/gtest.h"

namespace memesql {

TEST(BinOpExpressionTest, AddIntegers) {
    BinaryOpExpr expression(ArithmeticOperator::ADD,
                               std::make_shared<ConstantExpression>(Cell(2)),
                               std::make_shared<ConstantExpression>(Cell(3)));
    Record record{ {{"a", Cell(2)}, {"b", Cell(3)}} };
    Cell result = expression.evaluate(record);
    EXPECT_EQ(result.get<Int>(), 5);
}

TEST(BinOpExpressionTest, SubtractIntegers) {
    BinaryOpExpr expression(ArithmeticOperator::SUB,
                               std::make_shared<ConstantExpression>(Cell(5)),
                               std::make_shared<ConstantExpression>(Cell(2)));
    Record record;
    Cell result = expression.evaluate(record);
    EXPECT_EQ(result.get<Int>(), 3);
}

TEST(BinOpExpressionTest, MultiplyIntegers) {
    BinaryOpExpr expression(ArithmeticOperator::MUL,
                               std::make_shared<ConstantExpression>(Cell(4)),
                               std::make_shared<ConstantExpression>(Cell(5)));
    Record record;
    Cell result = expression.evaluate(record);
    EXPECT_EQ(result.get<Int>(), 20);
}

TEST(BinOpExpressionTest, DivideIntegers) {
    BinaryOpExpr expression(ArithmeticOperator::DIV,
                               std::make_shared<ConstantExpression>(Cell(10)),
                               std::make_shared<ConstantExpression>(Cell(2)));
    Record record;
    Cell result = expression.evaluate(record);
    EXPECT_EQ(result.get<Int>(), 5);
}

TEST(BinOpExpressionTest, ModulusIntegers) {
    BinaryOpExpr expression(ArithmeticOperator::MOD,
                               std::make_shared<ConstantExpression>(Cell(17)),
                               std::make_shared<ConstantExpression>(Cell(5)));
    Record record;
    Cell result = expression.evaluate(record);
    EXPECT_EQ(result.get<Int>(), 2);
}

TEST(BinOpExpressionTest, AddStrings) {
    BinaryOpExpr expression(ArithmeticOperator::ADD,
                               std::make_shared<ConstantExpression>(Cell("hello")),
                               std::make_shared<ConstantExpression>(Cell("world")));
    Record record;
    EXPECT_THROW(expression.evaluate(record), std::runtime_error);
}

TEST(BinOpExpressionTest, DivideByZero) {
    BinaryOpExpr expression(ArithmeticOperator::DIV,
                               std::make_shared<ConstantExpression>(Cell(10)),
                               std::make_shared<ConstantExpression>(Cell(0)));
    Record record;
    EXPECT_THROW(expression.evaluate(record), std::runtime_error);
}
} // namespace memesql