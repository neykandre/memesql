#pragma once

#include "Exceptions.hpp"
#include "Record.hpp"
#include <memory>
#include <unordered_map>

namespace memesql {

class WrappedRecord {
  public:
    WrappedRecord(std::shared_ptr<internal::Record> record,
                  std::unordered_map<std::string, size_t>&& column_indices)
        : m_record(record),
          m_column_indices(std::move(column_indices)) {};

    const Cell& get_cell(const std::string& column_name) const {
      if (!m_column_indices.contains(column_name)) {
            throw DBException("Unknown column name '" + column_name + "'");
        }
        return m_record.lock()->get_cell(m_column_indices.at(column_name));
    }

    template <Cell_t T>
    const T& get(const std::string& column_name) const {
        return get_cell(column_name).get<T>();
    }


  private:
    std::weak_ptr<internal::Record> m_record;
    const std::unordered_map<std::string, size_t> m_column_indices;
};
} // namespace memesql