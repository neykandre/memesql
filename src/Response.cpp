#include "Response.hpp"

namespace memesql {

ThinnedTable::ThinnedTable(std::shared_ptr<Table> table,
                           std::vector<size_t> rows_indices,
                           std::vector<std::string> columns_names)
    : m_table(table),
      m_rows_indices(rows_indices),
      m_columns_names(columns_names) {
}

std::shared_ptr<const Table> ThinnedTable::get_table() const {
    return m_table;
}

const std::vector<size_t>& ThinnedTable::get_rows_indices() const {
    return m_rows_indices;
}

const std::vector<std::string>& ThinnedTable::get_columns_names() const {
    return m_columns_names;
}

Response::Response(std::vector<ThinnedTable> tables, size_t records_count,
                   bool is_ok)
    : m_tables(tables),
      m_records_count(records_count),
      m_is_ok(is_ok) {
}

Response::Response(bool is_ok)
    : m_is_ok(is_ok) {
}

ResponseIterator Response::begin() const {
    return ResponseIterator(*this, 0, 0);
}

ResponseIterator Response::end() const {
    return ResponseIterator(*this, m_tables.size(), 0);
}

bool Response::is_ok() const {
    return m_is_ok;
}

size_t Response::records_count() const {
    return m_records_count;
}

} // namespace memesql