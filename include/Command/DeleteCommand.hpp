#pragma once

#include "Command.hpp"
#include "Expression/Expression.hpp"
#include "Response.hpp"
#include "SelectCommand.hpp"

namespace memesql {
class DeleteCommand : public Command {
  public:
    DeleteCommand(std::string table_name, std::shared_ptr<Expression> expr)
        : m_table_name(table_name),
          m_condition(expr) {
    }

    Response execute(DataBase& db) override {
        if (!db.m_tables.contains(m_table_name)) {
            throw CommandException("No such table '" + m_table_name + "'");
        }

        auto&& iterators =
            SelectCommand{ { { m_table_name, "*" } }, m_table_name, m_condition }
                .get_iterators(db);

        for (auto&& it : iterators) {
            db.m_tables.at(m_table_name)->delete_record(it);
        }

        return Response{};
    }

  private:
    std::string m_table_name;
    std::shared_ptr<Expression> m_condition;
};
} // namespace memesql