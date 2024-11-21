#include "Record.hpp"

namespace memesql {

Record::Record(std::map<std::string, Cell> cells)
    : m_cells(cells) {
}

const Cell& Record::operator[](const std::string& column_name) const {
    if (!m_cells.contains(column_name)) {
        throw std::invalid_argument("Unknown column name");
    }

    return m_cells.at(column_name);
}
} // namespace memesql