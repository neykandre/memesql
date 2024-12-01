#include "Response.hpp"

namespace memesql {

Response::Response(std::vector<internal::ThinnedTable> tables, size_t records_count)
    : m_tables(tables),
      m_records_count(records_count) {
}

ResponseIterator Response::begin() const {
    if (m_records_count == 0) {
        return end();
    }
    return ResponseIterator(&m_tables, 0, 0);
}

ResponseIterator Response::end() const {
    return ResponseIterator(&m_tables, m_tables.size(), 0);
}

size_t Response::records_count() const {
    return m_records_count;
}

} // namespace memesql