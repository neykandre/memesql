#pragma once

#include "Command/Command.hpp"
#include "Command/CreateCommand.hpp"
#include "Command/InsertCommand.hpp"
#include "Definitions.hpp"
#include "Expression/Expression.hpp"
#include "Lexer.hpp"
#include "Parser/Token.hpp"
#include <cstring>
#include <memory>
#include <string>

namespace memesql {
class Parser {
  public:
    std::shared_ptr<Command> parse(const std::string& query);

  private:
    std::shared_ptr<Expression> parse_expression();

    std::shared_ptr<CreateCommand> parse_create();
    Table::Header parse_header();
    ColumnFields::Attributes parse_attributes();

    std::shared_ptr<Command> parse_select();

    std::shared_ptr<InsertCommand> parse_insert();
    std::optional<InsertCommand::Map> try_parse_map();
    std::optional<InsertCommand::Vector> try_parse_vector();

    std::shared_ptr<Command> parse_update();

    std::shared_ptr<Command> parse_delete();

    std::shared_ptr<Command> parse_drop();

    Token expect(Token::Type type);

    template <typename... Types>
    Token expect_one_of(Types... types) {
        static_assert((std::is_same_v<Types, Token::Type> && ...),
                      "All arguments to expect_one_of must be of type Token::Type");
        size_t pos  = m_lexer->get_pos();
        Token token = m_lexer->get_next_token();
        if (!((token.get_type() == types) || ...)) {
            std::string message = "expected one of ";

            auto append_type_name = [&](Token::Type type) {
                if (type > Token::Type::KEYWORDS_START &&
                    type < Token::Type::KEYWORDS_END) {
                    return "keyword '" + m_lexer->get_token_name(type) + "'";
                } else if (type > Token::Type::OPERATORS_START &&
                           type < Token::Type::OPERATORS_END) {
                    return "operator '" + m_lexer->get_token_name(type) + "'";
                } else if (type > Token::Type::PUNCTUATION_START &&
                           type < Token::Type::PUNCTUATION_END) {
                    return "'" + m_lexer->get_token_name(type) + "'";
                } else if (type == Token::Type::STRING_LITERAL) {
                    return std::string{ "string literal" };
                } else if (type == Token::Type::NUMBER_LITERAL) {
                    return std::string{ "integer literal" };
                } else if (type == Token::Type::BYTES_LITERAL) {
                    return std::string{ "bytes literal" };
                } else if (type == Token::Type::IDENTIFIER) {
                    return std::string{ "identifier" };
                } else if (type == Token::Type::END_OF_FILE) {
                    return std::string{ "end of file" };
                } else {
                    return std::string{ "unknown token type" };
                }
            };

            size_t offset = std::strlen("^ expected one of ");
            ((message += append_type_name(types) + ", \n" +
                         std::string(token.get_pos() + offset, ' ')),
             ...);
            for (size_t i = 0; i < offset - 1; ++i) {
                message.pop_back();
            }
            message += "BUT got " + append_type_name(token.get_type());
            throw ParserException(message, m_lexer->get_input(), token.get_pos());
        }
        return token;
    }

    template <typename... Types>
    Token try_expect_one_of(Types... types) {
        size_t pos = m_lexer->get_pos();
        try {
            return expect_one_of(types...);
        } catch (ParserException& e) {
            m_lexer->set_pos(pos);
            return Token(Token::Type::PUNCTUATION_END, pos);
        }
    }

    Token try_expect(Token::Type type);

    bool check_expect(Token token) const;

  private:
    std::unique_ptr<Lexer> m_lexer;
};
} // namespace memesql