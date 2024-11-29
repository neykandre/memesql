#include "ResponseIterator.hpp"
#include "Record.hpp"
#include "Table.hpp"
#include "WrappedRecord.hpp"
#include <unordered_map>

namespace memesql {

ResponseIterator::ResponseIterator(
    const ResponseIterator::ThTableVector* th_table_vec, size_t table_index,
    size_t record_it_index) noexcept
    : m_th_table_vec(th_table_vec),
      m_table_index(table_index),
      m_record_it_index(record_it_index) {
}

ResponseIterator::ResponseIterator(const ResponseIterator& other) noexcept
    : m_th_table_vec(other.m_th_table_vec),
      m_table_index(other.m_table_index),
      m_record_it_index(other.m_record_it_index) {
}

ResponseIterator& ResponseIterator::operator=(ResponseIterator other) {
    std::swap(m_th_table_vec, other.m_th_table_vec);
    std::swap(m_table_index, other.m_table_index);
    std::swap(m_record_it_index, other.m_record_it_index);
    return *this;
}

ResponseIterator& ResponseIterator::operator++() {
    ++m_record_it_index;

    if (m_record_it_index ==
        m_th_table_vec->at(m_table_index).get_rows_iterators().size()) {
        ++m_table_index;
        m_record_it_index = 0;
    }

    return *this;
}

ResponseIterator ResponseIterator::operator++(int) {
    ResponseIterator tmp = *this;
    ++*this;
    return tmp;
}

ResponseIterator::value_type ResponseIterator::operator*() const {
    std::unordered_map<std::string, size_t> column_indices;

    auto&& table = m_th_table_vec->at(m_table_index);

    auto&& columns_names = table.get_columns_names();

    for (auto&& column_name : columns_names) {
        column_indices[column_name] =
            table.get_table()->get_header().columns.at(column_name).index;
    }

    return WrappedRecord(*table.get_rows_iterators().at(m_record_it_index),
                         std::move(column_indices));
}

bool ResponseIterator::operator==(const ResponseIterator& other) const {
    return m_table_index == other.m_table_index &&
           m_record_it_index == other.m_record_it_index &&
           m_th_table_vec == other.m_th_table_vec;
}

bool ResponseIterator::operator!=(const ResponseIterator& other) const {
    return !(*this == other);
}

} // namespace memesql