#pragma once

#include "ResponseIterator.hpp"
#include "FixedResponseIterator.hpp"
#include "ThinnedTable.hpp"
#include <ranges>
#include <vector>

namespace memesql {

class Response {

    friend class ResponseIterator;
    
    template<int size>
    friend class FixedResponseIterator;

  public:
    Response() = default;
    explicit Response(std::vector<internal::ThinnedTable> tables, size_t records_count);

    ResponseIterator begin() const;
    ResponseIterator end() const;

    size_t records_count() const;

    template <int size>
    auto get() {
        return std::ranges::subrange(
            FixedResponseIterator<size>(&m_tables, 0, 0),
            FixedResponseIterator<size>(&m_tables, m_tables.size(), 0));
    }

    // const std::vector<ThinnedTable>& get_tables() const {
    //   return m_tables;
    // }

  private:
    std::vector<internal::ThinnedTable> m_tables;
    size_t m_records_count;
};

} // namespace memesql