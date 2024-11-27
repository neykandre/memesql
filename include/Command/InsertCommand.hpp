#pragma once

#include "Command.hpp"
#include "Definitions.hpp"
#include <unordered_map>

namespace memesql {
class InsertCommand : public Command {

  public:
    using Map           = std::unordered_map<std::string, Cell>;
    using Vector        = std::vector<Cell>;
    using InsertionType = std::variant<Vector, Map>;
    InsertCommand(InsertionType insertion, std::string table_name)
        : m_insertion(insertion),
          m_table_name(table_name) {
    }

    Response execute(DataBase& db) override {
        if (!db.m_tables.contains(m_table_name)) {
            throw CommandException("Table does not exist");
        }

        Map cells;
        if (std::holds_alternative<Vector>(m_insertion)) {
            cells = construct_map_from_vector(db);
        } else {
            cells = std::get<Map>(m_insertion);
        }

        check_types(db, cells);

        db.m_tables[m_table_name]->create_record(cells);

        size_t row_index = db.m_tables[m_table_name]->get_all_records().size() - 1;
        std::vector<std::string> columns_names;

        for (auto&& [column_name, column] :
             db.m_tables[m_table_name]->get_header().columns) {
            columns_names.push_back(column_name);
        }

        return Response{ { ThinnedTable{ db.m_tables[m_table_name],
                                         { row_index },
                                         columns_names } },
                         1 };
    }

  private:
    InsertionType m_insertion;
    std::string m_table_name;

    Map construct_map_from_vector(const DataBase& db) {

        Map cells;
        auto insertion = std::get<Vector>(m_insertion);
        auto header    = db.m_tables.at(m_table_name)->get_header();

        for (auto&& [column_name, column] : header.columns) {
            if (column.index < insertion.size()) {
                cells[column_name] = insertion[column.index];
            }
        }
        return cells;
    }

    void check_types(const DataBase& db, const Map& cells) {
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
};
} // namespace memesql