#pragma once
#include <string>
#include <vector>

namespace memesql {
class Parser {
  public:

    Query parse(const std::string& query);
};
} // namespace memesql