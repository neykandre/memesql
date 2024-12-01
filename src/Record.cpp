#include "Record.hpp"
#include "Exceptions.hpp"
#include <cstddef>

namespace memesql::internal {

Record::Record(std::vector<Cell> cells)
    : m_cells(cells) {
}

const Cell& Record::get_cell(size_t index) const {
    return m_cells.at(index);
}

Cell& Record::get_cell(size_t index) {
    return m_cells.at(index);
}
} // namespace memesql