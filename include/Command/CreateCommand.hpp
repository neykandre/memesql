#pragma once

#include "Checker.hpp"
#include "Command.hpp"
#include "Definitions.hpp"
#include "Exceptions.hpp"

namespace memesql::internal {
class CreateCommand : public Command {
  public:
    CreateCommand(std::string table_name, Table::Header header)
        : m_table_name(table_name),
          m_header(header) {
    }

    Response execute(DataBase& db) override {
        Checker::check_table_exists({db.m_tables, m_table_name});

        Checker::check_header(m_header);

        db.m_tables[m_table_name] = std::make_shared<Table>(m_header);
        return Response{};
    }

  private:
    std::string m_table_name;
    Table::Header m_header;
};

} // namespace memesql