#include "gtest/gtest.h"
#include "../include/Cell.hpp"

using namespace memesql;

TEST(CellTest, AddIntegers) {
    Cell c1(1);
    Cell c2(2);
    Cell result = c1 + c2;
    EXPECT_EQ(result.get<int>(), 3);
}

TEST(CellTest, AddStrings) {
    Cell c1("hello");
    Cell c2("world");
    Cell result = c1 + c2;
    EXPECT_EQ(result.get<std::string>(), "helloworld");
}

TEST(CellTest, AddIntAndString) {
    Cell c1(1);
    Cell c2("hello");
    EXPECT_THROW(c1 + c2, std::invalid_argument);
}

TEST(CellTest, AddNullAndInt) {
    Cell c1;
    Cell c2(1);
    EXPECT_THROW(c1 + c2, std::invalid_argument);
}

TEST(CellTest, AddNullAndString) {
    Cell c1;
    Cell c2("hello");
    EXPECT_THROW(c1 + c2, std::invalid_argument);
}

TEST(CellTest, CompareIntegers) {
    Cell c1(1);
    Cell c2(2);
    EXPECT_LT(c1, c2);
    EXPECT_LE(c1, c2);
    EXPECT_GT(c2, c1);
    EXPECT_GE(c2, c1);
    EXPECT_EQ(c1, c1);
    EXPECT_NE(c1, c2);
}

TEST(CellTest, CompareStrings) {
    Cell c1("abc");
    Cell c2("def");
    EXPECT_LT(c1, c2);
    EXPECT_LE(c1, c2);
    EXPECT_GT(c2, c1);
    EXPECT_GE(c2, c1);
    EXPECT_EQ(c1, c1);
    EXPECT_NE(c1, c2);
}

TEST(CellTest, CompareBools) {
    Cell c1(true);
    Cell c2(false);
    EXPECT_GT(c1, c2);
    EXPECT_GE(c1, c2);
    EXPECT_LT(c2, c1);
    EXPECT_LE(c2, c1);
    EXPECT_EQ(c1, c1);
    EXPECT_NE(c1, c2);
}

TEST(CellTest, CompareMixedTypes) {
    Cell c1(1);
    Cell c2("abc");
    EXPECT_THROW(c1 < c2, std::invalid_argument);
    EXPECT_THROW(c1 > c2, std::invalid_argument);
    EXPECT_THROW(c1 == c2, std::invalid_argument);
    EXPECT_THROW(c1 != c2, std::invalid_argument);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}