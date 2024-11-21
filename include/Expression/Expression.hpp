#pragma once

#include "../Cell.hpp"
#include "../Record.hpp"

namespace memesql {

class Expression {
  public:
    virtual ~Expression() = default;

    virtual Cell evaluate(const Record&) const = 0;

    virtual bool is_constant() const { return false; }
    virtual bool is_column() const { return false; }
};
} // namespace memesql