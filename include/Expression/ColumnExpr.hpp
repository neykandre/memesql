#pragma once

#include "../Record.hpp"
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
    Cell evaluate(std::shared_ptr<Table> table, size_t row) const override {
        size_t index = table->get_header().columns.at(m_column_name).index;
        return table->get_record(row)->get_cell(index);
    }

    bool is_column() const override {
        return true;
    }

  private:
    std::string m_column_name;
};
} // namespace memesql