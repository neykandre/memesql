#include "DataBase.hpp"
#include "Response.hpp"
#include "Parser.hpp"

namespace memesql {

void DataBase::create_table(const std::string& table_name, const Table::Header& header) {
    if (m_tables.contains(table_name)) {
        throw std::invalid_argument("Table already exists");
    }
    m_tables[table_name] = std::make_shared<Table>(header);
}

void DataBase::drop_table(const std::string& table_name) {
    if (!m_tables.contains(table_name)) {
        throw std::invalid_argument("Table does not exist");
    }
    m_tables.erase(table_name);
}

Response DataBase::execute(const std::string& query) {
    Parser parser;
    Query parsed_query = parser.parse(query);
    
    
}

}