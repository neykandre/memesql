#pragma once

#include "../DataBase.hpp"
#include "../Response.hpp"
#include "Command.hpp"

namespace memesql {
class CreateCommand : public Command {
  public:
    CreateCommand(std::string table_name, Table::Header header)
        : m_table_name(table_name),
          m_header(header) {
    }

    Response execute(DataBase& db) override {
        // if (db.m_tables.contains(m_table_name)) {
        //     throw std::invalid_argument("Table already exists");
        //     return Response(nullptr, 0, false);
        // } else {
        //     db.m_tables[m_table_name] = std::make_shared<Table>(m_header);
        //     return Response(nullptr, 0, true);
        // }
    }

  private:
    std::string m_table_name;
    Table::Header m_header;
};

} // namespace memesql