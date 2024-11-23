#include "Record.hpp"
#include "Exceptions.hpp"

namespace memesql {

Record::Record(std::vector<Cell> cells) : m_cells(cells) {}

const Cell& Record::get_cell(size_t index) const {
    if (index >= m_cells.size()) {
        throw DBException("Index out of range");
    }

    return m_cells[index];
}
} // namespace memesql