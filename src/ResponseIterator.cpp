#include "ResponseIterator.hpp"
#include "Response.hpp"
#include "WrappedRecord.hpp"
#include <unordered_map>

namespace memesql {

ResponseIterator::ResponseIterator(const Response& response, size_t table_index,
                                   size_t record_index)
    : m_response(response),
      m_table_index(table_index),
      m_record_index(record_index) {
}

ResponseIterator ResponseIterator::operator++() {
    ++m_record_index;

    if (m_record_index ==
        m_response.m_tables[m_table_index].get_rows_indices().size()) {
        ++m_table_index;
        m_record_index = 0;
    }

    return *this;
}

ResponseIterator ResponseIterator::operator++(int) {
    ResponseIterator tmp = *this;
    ++*this;
    return tmp;
}

WrappedRecord ResponseIterator::operator*() {
    std::unordered_map<std::string, size_t> column_indices;

    auto&& table = m_response.m_tables[m_table_index];

    auto&& columns_names = table.get_columns_names();

    for (auto&& column_name : columns_names) {
        column_indices[column_name] =
            table.get_table()->get_header().columns.at(column_name).index;
    }

    return WrappedRecord(
        table.get_table()->get_record(table.get_rows_indices()[m_record_index]),
        std::move(column_indices));
}

WrappedRecord ResponseIterator::operator->() {
    return operator*();
}

bool ResponseIterator::operator==(const ResponseIterator& other) const {
    return m_table_index == other.m_table_index &&
           m_record_index == other.m_record_index &&
           &m_response == &other.m_response;
}

bool ResponseIterator::operator!=(const ResponseIterator& other) const {
    return !(*this == other);
}

} // namespace memesql