#pragma once

#include "Cell.hpp"
#include "Exceptions.hpp"
#include "ResponseIterator.hpp"
#include "ThinnedTable.hpp"
#include <array>
#include <iostream>
#include <utility>

namespace memesql {
template <int size>
class FixedResponseIterator : public ResponseIterator {
  public:
    using value_type        = std::array<Cell, size>;
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;

    using ResponseIterator::ResponseIterator;

    FixedResponseIterator<size>& operator=(FixedResponseIterator<size> other) {
        std::swap(m_th_table_vec, other.m_th_table_vec);
        std::swap(m_table_index, other.m_table_index);
        std::swap(m_record_it_index, other.m_record_it_index);

        return *this;
    }

    FixedResponseIterator<size>& operator++() {
        ResponseIterator::operator++();
        return *this;
    }

    FixedResponseIterator<size> operator++(int) {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    value_type operator*() const {
        std::array<Cell, size> ret_arr;
        auto wrapped_record = ResponseIterator::operator*();

        auto&& ordered_columns_names =
            m_th_table_vec->at(m_table_index).get_columns_names();

        if (ordered_columns_names.size() > size) {
            throw DBException("not enough arguments to unpack");
        }

        if (ordered_columns_names.size() < size) {
            throw DBException("to many arguments to unpack");
        }

        for (size_t i = 0; i < size; ++i) {
            ret_arr.at(i) = wrapped_record.get_cell(ordered_columns_names.at(i));
        }

        return ret_arr;
    }
};
} // namespace memesql
