#pragma once

#include "ThinnedTable.hpp"

namespace memesql {
ThinnedTable::ThinnedTable(std::shared_ptr<Table> table,
                           std::vector<TableIterator> rows_iterators,
                           std::vector<std::string> columns_names)
    : m_table(table),
      m_rows_iterators(rows_iterators),
      m_columns_names(columns_names) {
}

std::shared_ptr<const Table> ThinnedTable::get_table() const {
    return m_table;
}

const std::vector<ThinnedTable::TableIterator>& ThinnedTable::get_rows_iterators() const {
    return m_rows_iterators;
}

const std::vector<std::string>& ThinnedTable::get_columns_names() const {
    return m_columns_names;
}
} // namespace memesql