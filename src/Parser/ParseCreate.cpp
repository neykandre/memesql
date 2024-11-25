#include "Parser/Parser.hpp"

namespace memesql {

std::shared_ptr<CreateCommand> Parser::parse_create() {
    expect(Token::Type::TABLE);

    Token token            = expect(Token::Type::IDENTIFIER);
    std::string table_name = token.get_value<std::string>();

    Table::Header header = parse_header();

    expect_one_of(Token::Type::SEMICOLON, Token::Type::END_OF_FILE);

    return std::make_shared<CreateCommand>(table_name, header);
}

Table::Header Parser::parse_header() {
    Table::Header header;

    Token token = expect(Token::Type::LEFT_PARENTHESIS);
    while (token.get_type() != Token::Type::RIGHT_PARENTHESIS) {
        token = expect_one_of(Token::Type::IDENTIFIER, Token::Type::LEFT_BRACE);
        Table::Header::Column column;

        if (token.get_type() == Token::Type::LEFT_BRACE) {
            column.attributes = parse_attributes();
            token             = expect(Token::Type::IDENTIFIER);
        }

        std::string name = token.get_value<std::string>();
        expect(Token::Type::COLON);
        token = expect_one_of(Token::Type::INT, Token::Type::STRING,
                              Token::Type::BOOL, Token::Type::BYTES);
        if (token.get_type() == Token::Type::INT) {
            column.type = ColumnFields::DataTypes::INT;
        }
        if (token.get_type() == Token::Type::STRING) {
            column.type = ColumnFields::DataTypes::STRING;
        }
        if (token.get_type() == Token::Type::BOOL) {
            column.type = ColumnFields::DataTypes::BOOL;
        }
        if (token.get_type() == Token::Type::BYTES) {
            column.type = ColumnFields::DataTypes::BYTES;
        }
        if (column.type == ColumnFields::DataTypes::BYTES ||
            column.type == ColumnFields::DataTypes::STRING) {
            expect(Token::Type::LEFT_BRACKET);
            token         = expect(Token::Type::NUMBER_LITERAL);
            column.length = token.get_value<int>();
            expect(Token::Type::RIGHT_BRACKET);
        }

        token = expect_one_of(Token::Type::COMMA, Token::Type::EQUAL,
                              Token::Type::RIGHT_PARENTHESIS);

        if (token.get_type() == Token::Type::EQUAL) {
            token = expect_one_of(Token::Type::NUMBER_LITERAL,
                                  Token::Type::STRING_LITERAL, Token::Type::TRUE,
                                  Token::Type::FALSE, Token::Type::NULL_LITERAL);

            if (token.get_type() == Token::Type::NUMBER_LITERAL) {
                column.default_value = token.get_value<int>();
            }
            if (token.get_type() == Token::Type::STRING_LITERAL) {
                column.default_value = token.get_value<std::string>();
            }
            if (token.get_type() == Token::Type::TRUE) {
                column.default_value = true;
            }
            if (token.get_type() == Token::Type::FALSE) {
                column.default_value = false;
            }
            if (token.get_type() == Token::Type::NULL_LITERAL) {
                column.default_value = Null{};
            }

            token =
                expect_one_of(Token::Type::COMMA, Token::Type::RIGHT_PARENTHESIS);
        }

        column.index         = header.columns.size();
        header.columns[name] = column;
    }
    return header;
}

ColumnFields::Attributes Parser::parse_attributes() {
    ColumnFields::Attributes attributes;

    Token token = expect_one_of(Token::Type::KEY, Token::Type::UNIQUE,
                                Token::Type::AUTOINCREMENT, Token::Type::NOT_NULL,
                                Token::Type::RIGHT_BRACE);

    while (token.get_type() != Token::Type::RIGHT_BRACE) {
        switch (token.get_type()) {
        case Token::Type::KEY:
            attributes.add_key();
            break;
        case Token::Type::UNIQUE:
            attributes.add_unique();
            break;
        case Token::Type::AUTOINCREMENT:
            attributes.add_autoincrement();
            break;
        case Token::Type::NOT_NULL:
            attributes.add_not_null();
            break;
        default:
            break;
        }
        token = expect_one_of(Token::Type::COMMA, Token::Type::RIGHT_BRACE);
        if (token.get_type() == Token::Type::COMMA) {
            token = expect_one_of(Token::Type::KEY, Token::Type::UNIQUE,
                                  Token::Type::AUTOINCREMENT, Token::Type::NOT_NULL);
        }
    }

    return attributes;
}

} // namespace memesql