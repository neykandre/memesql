#pragma once

#include "DataBase.hpp"
#include "Expression/Expression.hpp"
#include "InsertCommand.hpp"
#include "SelectCommand.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace memesql {
class UpdateCommand : public InsertCommand {
  public:
    UpdateCommand(Map update_map, std::string table_name,
                  std::shared_ptr<Expression> condition)
        : InsertCommand(update_map, table_name),
          m_condition(condition) {
    }

    Response execute(DataBase& db) override {
        if (!db.m_tables.contains(m_table_name)) {
            throw CommandException("No such table '" + m_table_name + "'");
        }

        auto&& table = db.m_tables.at(m_table_name);

        auto&& select_command =
            SelectCommand{ { { m_table_name, "*" } }, m_table_name, m_condition };

        auto&& iterators = select_command.get_iterators(db);

        for (auto&& record_it : iterators) {
            CellMap update_map;
            for (auto&& [column_name, expr] : std::get<Map>(m_insertion)) {
                update_map[column_name] = expr->evaluate(table, record_it);
            }
            check_types(db, update_map);

            table->update_record(record_it, update_map);
        }

        return Response{ { ThinnedTable{ db.m_tables.at(m_table_name), iterators,
                                         select_command.get_columns_names(db) } },
                         iterators.size() };
    }

  private:
    std::shared_ptr<Expression> m_condition;
};
} // namespace memesql