#pragma once

#include "Table.hpp"
#include <memory>

namespace memesql {

class Response {
    std::shared_ptr<Table> m_table;
    using iterator = decltype(m_table->m_records)::iterator;

    explicit Response(std::shared_ptr<Table> table);

  public:
    iterator begin();
    iterator end();
};
} // namespace memesql