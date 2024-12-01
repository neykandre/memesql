#pragma once

#include "Command/Command.hpp"
#include "Command/CreateCommand.hpp"
#include "Command/InsertCommand.hpp"
#include "Command/SelectCommand.hpp"
#include "Command/DeleteCommand.hpp"
#include "Command/UpdateCommand.hpp"
#include "Definitions.hpp"
#include "Expression/Expression.hpp"
#include "Lexer.hpp"
#include "Parser/Expector.hpp"
#include "Parser/Token.hpp"
#include <cstring>
#include <memory>
#include <string>

namespace memesql::internal {
class Parser {
  public:
    std::shared_ptr<Command> parse(const std::string& query);

  private:
    int get_precedence(Token::Type) const;
    std::shared_ptr<Expression> parse_expression();
    std::shared_ptr<Expression> parse_paren_expression(Token::Type);
    std::shared_ptr<Expression> parse_primary();
    std::shared_ptr<Expression> parse_value();
    std::shared_ptr<Expression> parse_binop_right(int, std::shared_ptr<Expression>);

    std::shared_ptr<CreateCommand> parse_create();
    Table::Header parse_header();
    ColumnFields::Attributes parse_attributes();

    std::shared_ptr<SelectCommand> parse_select();
    SelectCommand::Column parse_column();

    std::shared_ptr<InsertCommand> parse_insert();
    InsertCommand::Map parse_map();
    InsertCommand::Vector parse_vector();

    std::shared_ptr<UpdateCommand> parse_update();

    std::shared_ptr<DeleteCommand> parse_delete();

    std::shared_ptr<Command> parse_drop();

  private:
    std::shared_ptr<Lexer> m_lexer;
    std::unique_ptr<Expector> m_expector;

    static const std::unordered_map<Token::Type, int> m_precedence;
    static const std::unordered_map<Token::Type, BinaryOpType> m_binop_map;
};
} // namespace memesql