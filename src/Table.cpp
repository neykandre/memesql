#include "Table.hpp"
#include "Checker.hpp"
#include "Exceptions.hpp"
#include "Record.hpp"
#include <unordered_map>

namespace memesql::internal {
Table::Table(const Header& header)
    : m_header(header) {
}

const Table::Header& Table::get_header() const {
    return m_header;
}

const Table::RecordList& Table::get_all_records() const {
    return m_records;
}

Table::RecordList::const_iterator Table::add_record(std::vector<Cell> cells) {
    return m_records.insert(m_records.end(), std::make_shared<Record>(cells));
}

Table::RecordList::const_iterator Table::delete_record(
    RecordList::const_iterator it) {
    return m_records.erase(it);
}

Table::RecordList::const_iterator Table::update_record(
    RecordList::const_iterator it,
    std::unordered_map<std::string, Cell> update_cells) {

    for (auto&& [column_name, cell] : update_cells) {
        auto&& column = m_header.columns.at(column_name);
        (*it)->get_cell(column.index) = cell;
    }

    return it;
}

} // namespace memesql::internal