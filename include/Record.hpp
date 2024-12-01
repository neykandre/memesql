#pragma once

#include "Cell.hpp"
#include <vector>

namespace memesql::internal {

class Record {
  public:
    explicit Record(std::vector<Cell> cells);

    const Cell& get_cell(size_t) const;
    Cell& get_cell(size_t);

  private:
    std::vector<Cell> m_cells;
};

} // namespace memesql