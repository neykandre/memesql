#include "Table.hpp"
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

void Table::create_record(std::map<std::string, Cell> cells) {
    std::vector<Cell> record_cells(m_header.columns.size());
    for (auto&& [column_name, cell] : cells) {
        if (!m_header.columns.contains(column_name)) {
            throw DBException("Unknown column name");
        }
        record_cells[m_header.columns.at(column_name).index] = cell;
    }

    m_records.push_back(std::make_shared<Record>(record_cells));
}

void Table::delete_record(size_t index) {
    m_records.erase(m_records.begin() + index);
}

} // namespace memesql