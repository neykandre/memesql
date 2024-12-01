#pragma once

#include "Cell.hpp"
#include "Table.hpp"

namespace memesql::internal {

class Expression {
  public:
    virtual ~Expression() = default;

    virtual Cell evaluate(std::shared_ptr<Table>,
                          Table::RecordList::const_iterator) const = 0;

    virtual bool is_constant() const {
        return false;
    }
    virtual bool is_column() const {
        return false;
    }

    virtual bool is_record_depends() const = 0;
};
} // namespace memesql