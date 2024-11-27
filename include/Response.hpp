#pragma once

#include "ResponseIterator.hpp"
#include "Table.hpp"
#include <memory>
#include <vector>

namespace memesql {

class ThinnedTable {
  public:
    ThinnedTable() = default;
    explicit ThinnedTable(std::shared_ptr<Table> table,
                          std::vector<size_t> rows_indices,
                          std::vector<std::string> columns_names);

    std::shared_ptr<const Table> get_table() const;
    const std::vector<size_t>& get_rows_indices() const;
    const std::vector<std::string>& get_columns_names() const;

  private:
    std::shared_ptr<Table> m_table;
    std::vector<size_t> m_rows_indices;
    std::vector<std::string> m_columns_names;
};

class Response {

    friend class ResponseIterator;

  public:
    Response() = default;
    explicit Response(std::vector<ThinnedTable> tables, size_t records_count);
    
    ResponseIterator begin() const;
    ResponseIterator end() const;
    
    size_t records_count() const;

  private:
    std::vector<ThinnedTable> m_tables;
    size_t m_records_count;
};
} // namespace memesql