#include "DataBase.hpp"
#include "Response.hpp"
#include "Parser.hpp"
#include "Executor.hpp"

namespace memesql {
    Response DataBase::execute(const std::string& query) {
        Parser parser;
        auto parsed_query = parser.parse(query);
        Executor executor;
        executor.execute(parsed_query);
        return executor.get_result();

    }
}