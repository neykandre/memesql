#include "Response.hpp"

namespace memesql {

Response::Response(std::shared_ptr<Table> table)
    : m_table(table) {
}

Response::iterator Response::begin() {
    return m_table->m_records.begin();
}
Response::iterator Response::end() {
    return m_table->m_records.end();
}

} // namespace memesql