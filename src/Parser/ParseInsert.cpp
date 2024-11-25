#include "Command/InsertCommand.hpp"
#include "Parser/Parser.hpp"
#include "Parser/Token.hpp"
#include <cstddef>

namespace memesql {

std::shared_ptr<InsertCommand> Parser::parse_insert() {

    InsertCommand::InsertionType insertion;

    auto opt_map = try_parse_map();

    if (opt_map.has_value()) {
        insertion = opt_map.value();
    } else {
        auto opt_vector = try_parse_vector();
        if (opt_vector.has_value()) {
            insertion = opt_vector.value();
        } else {
            throw ParserException("Unknown insert syntax", m_lexer->get_input(),
                                  m_lexer->get_pos());
        }
    }

    expect(Token::Type::TO);
    Token token            = expect(Token::Type::IDENTIFIER);
    std::string table_name = token.get_value<std::string>();
    expect_one_of(Token::Type::SEMICOLON, Token::Type::END_OF_FILE);

    return std::make_shared<InsertCommand>(insertion, table_name);
}

std::optional<InsertCommand::Map> Parser::try_parse_map() {
    size_t pos = m_lexer->get_pos();

    InsertCommand::Map map;

    Token token = expect(Token::Type::LEFT_PARENTHESIS);

    while (token.get_type() != Token::Type::RIGHT_PARENTHESIS) {
        if (token.get_type() == Token::Type::LEFT_PARENTHESIS) {
            token = try_expect(Token::Type::IDENTIFIER);

            if (!check_expect(token)) {
                m_lexer->set_pos(pos);
                return std::nullopt;
            }

        } else {
            token = expect(Token::Type::IDENTIFIER);
        }

        std::string column_name = token.get_value<std::string>();

        token = expect(Token::Type::EQUAL);

        token = try_expect_one_of(
            Token::Type::STRING_LITERAL, Token::Type::NUMBER_LITERAL,
            Token::Type::BYTES_LITERAL, Token::Type::NULL_LITERAL, Token::Type::TRUE,
            Token::Type::FALSE);

        map[column_name] = token.get_cell();

        token = expect_one_of(Token::Type::COMMA, Token::Type::RIGHT_PARENTHESIS);
    }

    return map;
}

std::optional<InsertCommand::Vector> Parser::try_parse_vector() {
    size_t pos = m_lexer->get_pos();

    InsertCommand::Vector vector;

    Token token = expect(Token::Type::LEFT_PARENTHESIS);

    while (token.get_type() != Token::Type::RIGHT_PARENTHESIS) {
        token = try_expect_one_of(
            Token::Type::STRING_LITERAL, Token::Type::NUMBER_LITERAL,
            Token::Type::BYTES_LITERAL, Token::Type::NULL_LITERAL, Token::Type::TRUE,
            Token::Type::FALSE);

        if (!check_expect(token)) {
            if (token.get_prev_type() == Token::Type::LEFT_PARENTHESIS) {
                token = try_expect_one_of(Token::Type::COMMA,
                                          Token::Type::RIGHT_PARENTHESIS);
                if (!check_expect(token)) {
                    m_lexer->set_pos(pos);
                    return std::nullopt;
                }
            } else {
                token = expect_one_of(Token::Type::COMMA,
                                      Token::Type::RIGHT_PARENTHESIS);
            }
            if (token.get_type() == Token::Type::COMMA) {
                vector.push_back(Cell{});
            }
        } else {
            vector.push_back(token.get_cell());
            token =
                expect_one_of(Token::Type::COMMA, Token::Type::RIGHT_PARENTHESIS);
        }
    }

    return vector;
}

} // namespace memesql