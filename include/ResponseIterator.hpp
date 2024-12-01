#pragma once

#include "ThinnedTable.hpp"
#include "WrappedRecord.hpp"
#include <cstddef>

namespace memesql {

class Response;

class ResponseIterator {
public:
    using value_type = WrappedRecord;
    using difference_type = std::ptrdiff_t;
    using ThTableVector = std::vector<internal::ThinnedTable>;

    ResponseIterator() = default;
    ResponseIterator(const ThTableVector*, size_t, size_t) noexcept;
    ResponseIterator(const ResponseIterator& other) noexcept;
    ResponseIterator& operator=(ResponseIterator other);
    ~ResponseIterator() = default;

    bool operator==(const ResponseIterator& other) const;
    bool operator!=(const ResponseIterator& other) const;
    ResponseIterator& operator++();
    ResponseIterator operator++(int);

    value_type operator*() const;

protected:
    const ThTableVector* m_th_table_vec;
    size_t m_table_index;
    size_t m_record_it_index;
};

}