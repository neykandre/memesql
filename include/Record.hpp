#pragma once

#include "Cell.hpp"
#include <vector>

namespace memesql {

class Record {
  public:
    explicit Record(std::vector<Cell> cells);
    // template <Cell_t T>
    // const T& get(const std::string& column_name) const {
    //     if (!m_cells.contains(column_name)) {
    //         throw std::invalid_argument("Unknown column name");
    //     }

    //     return m_cells.at(column_name).get<T>();
    // }

    const Cell& get_cell(size_t) const;

  private:
    std::vector<Cell> m_cells;
};

} // namespace memesql