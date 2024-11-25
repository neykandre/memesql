#pragma once

#include "../DataBase.hpp"
#include "../Response.hpp"
#include "Command.hpp"
#include "Definitions.hpp"
#include "Exceptions.hpp"

namespace memesql {
class CreateCommand : public Command {
  public:
    CreateCommand(std::string table_name, Table::Header header)
        : m_table_name(table_name),
          m_header(header) {
    }

    Response execute(DataBase& db) override {
        if (db.m_tables.contains(m_table_name)) {
            throw CommandException("Table already exists");
            return Response(false);
        }

        for (auto&& [column_name, column] : m_header.columns) {
          if (column.attributes.is_autoincrement() && column.type != ColumnFields::DataTypes::INT) {
            throw CommandException("column has 'autoincrement' attribute, but type is not int32");
          }
        }

        db.m_tables[m_table_name] = std::make_shared<Table>(m_header);
        return Response(true);
    }

  private:
    std::string m_table_name;
    Table::Header m_header;
};

} // namespace memesql