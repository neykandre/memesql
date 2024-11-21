#pragma once

#include "Cell.hpp"
#include <memory>
#include <optional>
#include <string>

namespace memesql {
struct Condition {
    enum class Type {
        LOGICAL_AND,
        LOGICAL_OR,
        LOGICAL_NOT,
        COMPARISON
    };
    Type type;

    std::unique_ptr<Condition> left;
    std::unique_ptr<Condition> right;

    struct Comparison {
        struct Operand_t : std::variant<std::string, Cell> {
            constexpr bool is_column() const {
                return std::holds_alternative<std::string>(*this);
            }
            constexpr bool is_cell() const {
                return std::holds_alternative<Cell>(*this);
            }
        };
        
         enum class Operator {
            EQUAL,
            NOT_EQUAL,
            LESS,
            LESS_EQUAL,
            GREATER,
            GREATER_EQUAL
        };

        enum class OperandType {
            COLUMN,
            VALUE
        };

        Operand_t left_operand;

        Operator op;

        Operand_t right_operand;
    };
    std::optional<Comparison> comparison;

    static Condition createLogicalAnd(std::unique_ptr<Condition> left,
                                      std::unique_ptr<Condition> right) {
        return Condition{ Type::LOGICAL_AND, std::move(left), std::move(right),
                          std::nullopt };
    }

    static Condition createLogicalOr(std::unique_ptr<Condition> left,
                                     std::unique_ptr<Condition> right) {
        return Condition{ Type::LOGICAL_OR, std::move(left), std::move(right),
                          std::nullopt };
    }

    static Condition createLogicalNot(std::unique_ptr<Condition> operand) {
        return Condition{ Type::LOGICAL_NOT, std::move(operand), nullptr,
                          std::nullopt };
    }

    // static Condition createComparison(const std::string& column,
    //                                   Comparison::Operator op, const Cell& value) {
    //     return Condition{ Type::COMPARISON, nullptr, nullptr,
    //                       Comparison{ column, op, value } };
    // }
};
} // namespace memesql