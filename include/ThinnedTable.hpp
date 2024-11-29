#pragma once

#include "Table.hpp"

namespace memesql {
class ThinnedTable {
  public:
    using TableIterator = Table::RecordList::const_iterator;

    ThinnedTable() = default;
    explicit ThinnedTable(std::shared_ptr<Table> table,
                          std::vector<TableIterator> rows_iterators,
                          std::vector<std::string> columns_names);

    std::shared_ptr<const Table> get_table() const;
    const std::vector<TableIterator>& get_rows_iterators() const;
    const std::vector<std::string>& get_columns_names() const;

  private:
    std::shared_ptr<Table> m_table;
    std::vector<TableIterator> m_rows_iterators;
    std::vector<std::string> m_columns_names;
};
} // namespace memesql