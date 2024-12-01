#pragma once

#include "Command.hpp"
#include "Expression/Expression.hpp"
#include "Response.hpp"
#include "SelectCommand.hpp"

namespace memesql::internal {
class DeleteCommand : public Command {
  public:
    DeleteCommand(std::string table_name, std::shared_ptr<Expression> expr)
        : m_table_name(table_name),
          m_condition(expr) {
    }

    Response execute(DataBase& db) override {
        Checker::check_table_exists({db.m_tables, m_table_name});

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