#pragma once

#include "Cell.hpp"
#include <map>

namespace memesql {

class Record {

    std::map<std::string, Cell> m_cells;


  public:
    explicit Record(std::map<std::string, Cell> cells);
    // template <Cell_t T>
    // const T& get(const std::string& column_name) const {
    //     if (!m_cells.contains(column_name)) {
    //         throw std::invalid_argument("Unknown column name");
    //     }

    //     return m_cells.at(column_name).get<T>();
    // }

    const Cell& operator[](const std::string& column_name) const;
};

} // namespace memesql