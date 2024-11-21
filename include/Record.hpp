#pragma once

#include "Cell.hpp"
#include <map>

namespace memesql {

class Record {
    friend class Table;

    std::map<std::string, Cell> m_cells;

    explicit Record(std::map<std::string, Cell> cells) : m_cells(std::move(cells)) {};

  public:

    template <Cell_t T>
    const T& get(const std::string& column_name) const {
        if (!m_cells.contains(column_name)) {
            throw std::invalid_argument("Unknown column name");
        }

        return m_cells.at(column_name).get<T>();
    }

};

} // namespace memesql