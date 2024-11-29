#pragma once

#include "Command.hpp"
#include "Definitions.hpp"
#include "Exceptions.hpp"
#include "Expression/Expression.hpp"
#include <memory>
#include <unordered_map>

namespace memesql {
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
        if (!db.m_tables.contains(m_table_name)) {
            throw CommandException("Table does not exist");
        }

        Map expressions;
        if (std::holds_alternative<Vector>(m_insertion)) {
            expressions = construct_map_from_vector(db);
        } else {
            expressions = std::get<Map>(m_insertion);
        }

        CellMap cells;
        for (auto&& [column_name, expr] : expressions) {
            if (expr->is_record_depends()) {
                throw CommandException("not constant expression in insert");
            }
            cells[column_name] = expr->evaluate(db.m_tables.at(m_table_name), {});
        }

        check_types(db, cells);

        auto insert_it = db.m_tables[m_table_name]->create_record(cells);

        std::vector<std::string> columns_names;

        for (auto&& [column_name, column] :
             db.m_tables[m_table_name]->get_header().columns) {
            columns_names.push_back(column_name);
        }

        return Response{ { ThinnedTable{ db.m_tables[m_table_name],
                                         { insert_it },
                                         columns_names } },
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

    void check_types(const DataBase& db, const CellMap& cells) {
        auto table_columns = db.m_tables.at(m_table_name)->get_header().columns;
        for (auto&& [column_name, cell] : cells) {
            if (!table_columns.contains(column_name)) {
                throw CommandException("Unknown column name '" + column_name + "'");
            }

            ColumnFields::DataTypes column_type = table_columns.at(column_name).type;
            switch (column_type) {
            case ColumnFields::DataTypes::INT:
                if (!std::holds_alternative<Int>(cell.get_variant()) &&
                    !std::holds_alternative<Null>(cell.get_variant())) {
                    throw CommandException("Type mismatch: column " + column_name +
                                           " has type 'int32', but got value " +
                                           cell.to_string());
                }
                break;
            case ColumnFields::DataTypes::STRING:
                if (!std::holds_alternative<String>(cell.get_variant()) &&
                    !std::holds_alternative<Null>(cell.get_variant())) {
                    throw CommandException("Type mismatch: column " + column_name +
                                           " has type 'string', but got value " +
                                           cell.to_string());
                }
                break;
            case ColumnFields::DataTypes::BOOL:
                if (!std::holds_alternative<Bool>(cell.get_variant()) &&
                    !std::holds_alternative<Null>(cell.get_variant())) {
                    throw CommandException("Type mismatch: column " + column_name +
                                           " has type 'bool', but got value " +
                                           cell.to_string());
                }
                break;
            case ColumnFields::DataTypes::BYTES:
                if (!std::holds_alternative<Bytes>(cell.get_variant()) &&
                    !std::holds_alternative<Null>(cell.get_variant())) {
                    throw CommandException("Type mismatch: column " + column_name +
                                           " has type 'bytes', but got value " +
                                           cell.to_string());
                }
            default:
                break;
            }
        }
    }

  protected:
    InsertionType m_insertion;
    std::string m_table_name;
};
} // namespace memesql