#pragma once

#include "Expression.hpp"

namespace memesql {
class ColumnExpr : public Expression {
  public:
    explicit ColumnExpr(std::string column_name)
        : m_column_name(column_name) {
    }
    std::string get_column_name() const {
        return m_column_name;
    }
    Cell evaluate(const Record& record) const override {
        return record[m_column_name];
    }

    bool is_column() const override {
        return true;
    }

  private:
    std::string m_column_name;
};
} // namespace memesql