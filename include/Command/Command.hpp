#pragma once

#include "DataBase.hpp"
#include "Exceptions.hpp"
#include "Response.hpp"
#include "Checker.hpp"

namespace memesql::internal {
class Command {
    friend class Parser;

  public:
    virtual ~Command()                  = default;
    virtual Response execute(DataBase&) = 0;
};
} // namespace memesql