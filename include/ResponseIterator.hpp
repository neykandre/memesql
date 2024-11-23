#pragma once

#include "WrappedRecord.hpp"

namespace memesql {

class Response;

class ResponseIterator {
public:
    ResponseIterator(const Response& response, size_t table_index, size_t record_index);

    bool operator==(const ResponseIterator& other) const;
    bool operator!=(const ResponseIterator& other) const;
    ResponseIterator operator++();
    ResponseIterator operator++(int);

    WrappedRecord operator*();
    WrappedRecord operator->();

private:
    const Response& m_response;
    size_t m_table_index;
    size_t m_record_index;
};

}