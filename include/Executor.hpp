#pragma once
#include "Parser.hpp"
#include <memory>

namespace memesql {

using Result = std::vector<std::shared_ptr<WrappedRecord>>;
class Executor {
    Result m_result;

    void 

  public:
    Executor();
    void execute(const Parser::Query& query);

    Result get_result() const;
};
} // namespace memesql