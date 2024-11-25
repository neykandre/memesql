#include "Table.hpp"
#include "Exceptions.hpp"
#include "Record.hpp"

namespace memesql {
Table::Table(const Header& header)
    : m_header(header) {
}

const Table::Header& Table::get_header() const {
    return m_header;
}

const Table::RecordList& Table::get_all_records() const {
    return m_records;
}

std::shared_ptr<Record> Table::get_record(size_t index) const {
    return m_records[index];
}

void Table::create_record(std::unordered_map<std::string, Cell> cells) {
    std::vector<Cell> record_cells(m_header.columns.size());

    // check for autocomplete
    for (auto&& [column_name, column] : m_header.columns) {
        if (!cells.contains(column_name) || cells.at(column_name).is_null()) {
            if (column.default_value.has_value()) {
                record_cells[column.index] = column.default_value.value();
            } else if (column.attributes.is_autoincrement()) {
                if (m_records.empty()) {
                    record_cells[column.index] = 0;
                } else {
                    record_cells[column.index] =
                        m_records.back()->get_cell(column.index) + 1;
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

    for (auto&& [column_name, cell] : cells) {
        if (!cell.is_null() &&
            (m_header.columns.at(column_name).attributes.is_key() ||
             m_header.columns.at(column_name).attributes.is_unique())) {
            for (auto&& record : m_records) {
                if (record->get_cell(m_header.columns.at(column_name).index) ==
                    cell) {
                    throw CommandException("Duplicate unique value");
                }
            }
        }
        if (!cell.is_null()) {
            record_cells[m_header.columns.at(column_name).index] = cell;
        }
    }

    m_records.push_back(std::make_shared<Record>(record_cells));
}

void Table::delete_record(size_t index) {
    m_records.erase(m_records.begin() + index);
}

} // namespace memesql