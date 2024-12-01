#include "DataBase.hpp"
#include "Response.hpp"
#include "Parser/Parser.hpp"

namespace memesql {
Response DataBase::execute(const std::string& query) {  
    internal::Parser parser;
    auto command = parser.parse(query);
    return command->execute(*this);
}

}