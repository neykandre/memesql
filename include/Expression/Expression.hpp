#pragma once

#include "../Cell.hpp"
#include "../Table.hpp"

namespace memesql {

class Expression {
  public:
    virtual ~Expression() = default;

    virtual Cell evaluate(std::shared_ptr<Table>, size_t row) const = 0;

    virtual bool is_constant() const { return false; }
    virtual bool is_column() const { return false; }
};
} // namespace memesql