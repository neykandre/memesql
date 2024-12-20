#pragma once

#include "Command.hpp"
#include "DataBase.hpp"
#include "Expression/Expression.hpp"
#include "Response.hpp"
#include "ThinnedTable.hpp"

namespace memesql::internal {

class SelectCommand : public Command {
  public:
    struct Column {
        std::string table_name;
        std::string column_name;
    };
    SelectCommand(std::vector<Column> columns, std::string table_name,
                  std::shared_ptr<Expression> condition)
        : m_columns(columns),
          m_table_name(table_name),
          m_condition(condition) {
    }

    Response execute(DataBase& db) override {
        Checker::check_table_exists({ db.m_tables, m_table_name });

        auto&& columns_names = get_columns_names(db);

        auto&& iterators = get_iterators(db);

        return Response{ { ThinnedTable{ db.m_tables.at(m_table_name), iterators,
                                         columns_names } },
                         iterators.size() };
    }

    std::vector<ThinnedTable::TableIterator> get_iterators(DataBase& db) {
        auto&& table = db.m_tables.at(m_table_name);
        std::vector<ThinnedTable::TableIterator> iterators;

        auto&& it = table->get_all_records().begin();
        for (; it != table->get_all_records().end(); ++it) {
            if (m_condition->evaluate(table, it)) {
                iterators.push_back(it);
            }
        }

        return iterators;
    }

    std::vector<std::string> get_columns_names(DataBase& db) {
        auto&& table = db.m_tables.at(m_table_name);
        std::vector<std::string> columns_names;

        for (auto&& [table_name, column_name] : m_columns) {
            if (table_name == m_table_name) {
                if (column_name == "*") {
                    size_t start_index = columns_names.size();
                    columns_names.resize(columns_names.size() +
                                         table->get_header().columns.size());

                    for (auto&& [table_column_name, column] :
                         table->get_header().columns) {
                        columns_names[start_index + column.index] =
                            table_column_name;
                    }
                } else {
                    columns_names.push_back(column_name);
                }
            }
        }

        return columns_names;
    }

  private:
    std::vector<Column> m_columns;
    std::string m_table_name;
    std::shared_ptr<Expression> m_condition;
};
} // namespace memesql::internal