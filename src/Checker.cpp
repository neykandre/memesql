#include "Checker.hpp"

namespace memesql::internal {
void Checker::check_table_exists(TableExistsDTO dto) {
    if (dto.is_creating ^ !dto.tables.contains(dto.table_name)) {
        throw CommandException(
            dto.is_creating ? ("Table '" + dto.table_name + "' already exists")
                            : ("No such table '" + dto.table_name + "'"));
    }
}

void Checker::check_types(CheckTypesDTO dto) {
    auto table_columns = dto.table->get_header().columns;
    for (auto&& [column_name, cell] : dto.cells) {
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

void Checker::check_header(const Table::Header& header) {
    for (auto&& [column_name, column] : header.columns) {
        if (column.attributes.is_autoincrement() &&
            column.type != ColumnFields::DataTypes::INT) {
            throw CommandException(
                "column has 'autoincrement' attribute, but type is not int32");
        }
    }
}

void Checker::check_is_const(std::shared_ptr<Expression> expr) {
    if (expr->is_record_depends()) {
        throw CommandException("not constant expression in insert");
    }
}

std::vector<Cell> Checker::autocomplete_record_map(AutocompleteDTO dto) {
    std::vector<Cell> record_cells(dto.header.columns.size());

    // check for autocomplete
    for (auto&& [column_name, column] : dto.header.columns) {
        if (!dto.input.contains(column_name) ||
            dto.input.at(column_name).is_null()) {
            if (column.default_value.has_value()) {
                record_cells[column.index] = column.default_value.value();
            } else if (column.attributes.is_autoincrement()) {
                if (dto.records.empty()) {
                    record_cells[column.index] = 0;
                } else {
                    record_cells[column.index] =
                        dto.records.back()->get_cell(column.index) + Cell{ 1 };
                }
            } else if (column.attributes.is_key()) {
                throw CommandException("column" + column_name +
                                       "has 'key' attribute, but missed");
            } else if (column.attributes.is_not_null()) {
                throw CommandException(
                    "column" + column_name +
                    "has 'not_null' attribute and hasn't default value, but missed");
            }
        }
    }

    return record_cells;
}

void Checker::check_duplicate(DuplicateDTO dto) {
    if (!dto.cell.is_null() &&
        (dto.attributes.is_key() || dto.attributes.is_unique())) {
        for (auto&& it = dto.records_begin; it != dto.records_end; ++it) {
            if (it->get()->get_cell(dto.column_index) == dto.cell) {
                throw CommandException("Duplicate unique value");
            }
        }
    }
}

} // namespace memesql::internal