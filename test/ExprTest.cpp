#include "../include/Expression/BinOpExpression.hpp"
#include "../include/Record.hpp"
#include "gtest/gtest.h"

namespace memesql {

TEST(BinOpExpressionTest, AddIntegers) {
    BinOpExpression expression(ArithmeticOperator::ADD,
                               std::make_shared<ConstantExpression>(Cell(2)),
                               std::make_shared<ConstantExpression>(Cell(3)));
    Record record{ {{"a", Cell(2)}, {"b", Cell(3)}} };
    Cell result = expression.evaluate(record);
    EXPECT_EQ(result.get<Int>(), 5);
}

TEST(BinOpExpressionTest, SubtractIntegers) {
    BinOpExpression expression(ArithmeticOperator::SUB,
                               std::make_shared<ConstantExpression>(Cell(5)),
                               std::make_shared<ConstantExpression>(Cell(2)));
    Record record;
    Cell result = expression.evaluate(record);
    EXPECT_EQ(result.get<Int>(), 3);
}

TEST(BinOpExpressionTest, MultiplyIntegers) {
    BinOpExpression expression(ArithmeticOperator::MUL,
                               std::make_shared<ConstantExpression>(Cell(4)),
                               std::make_shared<ConstantExpression>(Cell(5)));
    Record record;
    Cell result = expression.evaluate(record);
    EXPECT_EQ(result.get<Int>(), 20);
}

TEST(BinOpExpressionTest, DivideIntegers) {
    BinOpExpression expression(ArithmeticOperator::DIV,
                               std::make_shared<ConstantExpression>(Cell(10)),
                               std::make_shared<ConstantExpression>(Cell(2)));
    Record record;
    Cell result = expression.evaluate(record);
    EXPECT_EQ(result.get<Int>(), 5);
}

TEST(BinOpExpressionTest, ModulusIntegers) {
    BinOpExpression expression(ArithmeticOperator::MOD,
                               std::make_shared<ConstantExpression>(Cell(17)),
                               std::make_shared<ConstantExpression>(Cell(5)));
    Record record;
    Cell result = expression.evaluate(record);
    EXPECT_EQ(result.get<Int>(), 2);
}

TEST(BinOpExpressionTest, AddStrings) {
    BinOpExpression expression(ArithmeticOperator::ADD,
                               std::make_shared<ConstantExpression>(Cell("hello")),
                               std::make_shared<ConstantExpression>(Cell("world")));
    Record record;
    EXPECT_THROW(expression.evaluate(record), std::runtime_error);
}

TEST(BinOpExpressionTest, DivideByZero) {
    BinOpExpression expression(ArithmeticOperator::DIV,
                               std::make_shared<ConstantExpression>(Cell(10)),
                               std::make_shared<ConstantExpression>(Cell(0)));
    Record record;
    EXPECT_THROW(expression.evaluate(record), std::runtime_error);
}
} // namespace memesql