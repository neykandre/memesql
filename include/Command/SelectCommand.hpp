#pragma once

#include "Command.hpp"
#include "Definitions.hpp"
#include "Expression/Expression.hpp"
#include "Response.hpp"
#include <cstddef>

namespace memesql {

class SelectCommand : public Command {
  public:
    struct Column {
        std::string table_name;
        std::string column_name;
    };
    SelectCommand(std::vector<Column> columns, std::string table_name,
                  std::shared_ptr<Expression> expr)
        : m_columns(columns),
          m_table_name(table_name),
          m_expr(expr) {
    }

    Response execute(DataBase& db) override {
        if (!db.m_tables.contains(m_table_name)) {
            throw CommandException("No such table" + m_table_name);
        }

        auto table = db.m_tables.at(m_table_name);
        std::vector<std::string> columns_names;

        for (auto&& [table_name, column_name] : m_columns) {
            if (table_name == m_table_name) {
                columns_names.push_back(column_name);
            }
        }

        std::vector<size_t> rows_indices;
        for (size_t i = 0; i < table->get_all_records().size(); ++i) {
            if (m_expr->evaluate(table, i)) {
                rows_indices.push_back(i);
            }
        }

        return Response{ { ThinnedTable{ table, rows_indices, columns_names } },
                         rows_indices.size() };
    }

  private:
    std::vector<Column> m_columns;
    std::string m_table_name;
    std::shared_ptr<Expression> m_expr;
};
} // namespace memesql