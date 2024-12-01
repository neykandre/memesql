#pragma once

#include "Checker.hpp"
#include "DataBase.hpp"
#include "Expression/Expression.hpp"
#include "InsertCommand.hpp"
#include "SelectCommand.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace memesql::internal {
class UpdateCommand : public Command {
    using Map     = InsertCommand::Map;
    using CellMap = InsertCommand::CellMap;

  public:
    UpdateCommand(Map update_map, std::string table_name,
                  std::shared_ptr<Expression> condition)
        : m_update(update_map),
          m_table_name(table_name),
          m_condition(condition) {
    }

    Response execute(DataBase& db) override {
        Checker::check_table_exists({ db.m_tables, m_table_name });

        auto&& table = db.m_tables.at(m_table_name);

        auto&& select_command =
            SelectCommand{ { { m_table_name, "*" } }, m_table_name, m_condition };

        auto&& iterators = select_command.get_iterators(db);

        for (auto&& record_it : iterators) {
            CellMap update_map;
            for (auto&& [column_name, expr] : m_update) {
                update_map[column_name] = expr->evaluate(table, record_it);
            }
            Checker::check_types({ table, update_map });
            for (auto&& [column_name, column] : table->get_header().columns) {
                Checker::check_duplicate({ iterators.front(), iterators.back(),
                                           column.index, column.attributes,
                                           update_map.at(column_name) });
            }

            table->update_record(record_it, update_map);
        }

        return Response{ { ThinnedTable{ table, iterators,
                                         select_command.get_columns_names(db) } },
                         iterators.size() };
    }

  private:
    Map m_update;
    std::string m_table_name;
    std::shared_ptr<Expression> m_condition;
};
} // namespace memesql::internal