#pragma once

#include "Definitions.hpp"
#include "Expression/Expression.hpp"
#include <vector>
#include <string>
#include <memory>

namespace memesql {

struct Query {

    struct JoinCondition {
        JoinType type;
        std::string table_name;
        std::unique_ptr<Expression> on_condition;
    };

    QueryType type;
    std::string table_name;
    std::vector<std::string> columns_names;
    std::unique_ptr<Expression> where;
    std::vector<JoinCondition> joins;
};
} // namespace memesql