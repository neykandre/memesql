#pragma once

#include "Condition.hpp"

namespace memesql {

struct Query {
    enum class Type {
        SELECT,
        INSERT,
        UPDATE,
        DELETE,
        DROP
    };

    struct JoinCondition {
        enum class JoinType {
            INNER,
            LEFT,
            RIGHT,
            FULL
        };

        JoinType type;
        std::string table_name;
        std::string on_condition;
    };

    Type type;
    std::string table_name;
    std::vector<std::string> columns_names;
    Condition condition;
    std::vector<JoinCondition> joins;
};
} // namespace memesql