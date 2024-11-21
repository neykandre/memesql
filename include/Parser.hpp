#pragma once

#include "Query.hpp"
#include <memory>
#include <string>

namespace memesql {
class Parser {
  public:
    Query parse(const std::string& query);

  private:
    std::unique_ptr<Expression> parse_expression(const std::string& expression);
    
};
} // namespace memesql