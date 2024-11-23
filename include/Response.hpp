#pragma once

#include "Table.hpp"
#include <memory>

namespace memesql {

class Response {
  private:
    std::shared_ptr<Table> m_table;
    size_t m_records_count;
    bool m_is_ok;
    using iterator = Table::RecordList::iterator;

  public:
    explicit Response(std::shared_ptr<Table> table, size_t records_count, bool is_ok);
    iterator begin() const;
    iterator end() const;
    bool is_ok() const;
    size_t records_count() const;
};
} // namespace memesql