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

} // namespace memesql