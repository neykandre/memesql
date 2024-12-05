#pragma once

#include "Checker.hpp"
#include "Command.hpp"
#include "Definitions.hpp"
#include "Exceptions.hpp"
#include "Expression/Expression.hpp"
#include <memory>
#include <unordered_map>

namespace memesql::internal {
class InsertCommand : public Command {

  public:
    using Map    = std::unordered_map<std::string, std::shared_ptr<Expression>>;
    using Vector = std::vector<std::shared_ptr<Expression>>;
    using InsertionType = std::variant<Vector, Map>;
    using CellMap       = std::unordered_map<std::string, Cell>;
    InsertCommand(InsertionType insertion, std::string table_name)
        : m_insertion(insertion),
          m_table_name(table_name) {
    }

    Response execute(DataBase& db) override {
        Checker::check_table_exists({ db.m_tables, m_table_name });
        auto&& table = db.m_tables.at(m_table_name);

        Map expressions;
        if (std::holds_alternative<Vector>(m_insertion)) {
            expressions = construct_map_from_vector(db);
        } else {
            expressions = std::get<Map>(m_insertion);
        }

        CellMap cells;
        for (auto&& [column_name, expr] : expressions) {
            Checker::check_is_const(expr);
            cells[column_name] = expr->evaluate(table, {});
        }

        Checker::check_types({ table, cells });

        std::vector<Cell> record_cells = Checker::autocomplete_record_map(
            { cells, table->get_header(), table->get_all_records() });

        std::vector<std::string> columns_names(table->get_header().columns.size());
        for (auto&& [column_name, column] : table->get_header().columns) {
            columns_names[column.index] = column_name;
        }

        for (auto&& [column_name, cell] : cells) {
            auto&& column = table->get_header().columns.at(column_name);
            Checker::check_duplicate({ table->get_all_records().begin(),
                                       table->get_all_records().end(), column.index,
                                       column.attributes, cell });
            if (!cell.is_null()) {
                record_cells[column.index] = cell;
            }
        }

        auto insert_it = table->add_record(record_cells);

        return Response{ { ThinnedTable{ table, { insert_it }, columns_names } },
                         1 };
    }

  protected:
    Map construct_map_from_vector(const DataBase& db) {

        Map expressions;
        auto insertion = std::get<Vector>(m_insertion);
        auto header    = db.m_tables.at(m_table_name)->get_header();

        for (auto&& [column_name, column] : header.columns) {
            if (column.index < insertion.size()) {
                expressions[column_name] = insertion[column.index];
            }
        }
        return expressions;
    }

  protected:
    InsertionType m_insertion;
    std::string m_table_name;
};
} // namespace memesql::internal