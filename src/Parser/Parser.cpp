#include "Parser/Parser.hpp"
#include "Command/UpdateCommand.hpp"
#include "Definitions.hpp"
#include "Exceptions.hpp"
#include "Expression/BinaryOpExpr.hpp"
#include "Expression/ColumnExpr.hpp"
#include "Expression/ConstantExpr.hpp"
#include "Expression/Expression.hpp"
#include "Expression/UnaryOpExpr.hpp"
#include "Parser/Expector.hpp"
#include "Parser/Lexer.hpp"
#include "Parser/Token.hpp"
#include <memory>

#define OperatorTokens                                                              \
    Token::Type::PLUS, Token::Type::MINUS, Token::Type::MUL, Token::Type::DIV,      \
        Token::Type::MOD, Token::Type::EQUAL, Token::Type::NOT_EQUAL,               \
        Token::Type::LESS, Token::Type::LESS_EQUAL, Token::Type::GREATER_EQUAL,     \
        Token::Type::GREATER, Token::Type::AND, Token::Type::OR, Token::Type::XOR

#define LiteralTokens                                                               \
    Token::Type::NUMBER_LITERAL, Token::Type::STRING_LITERAL,                       \
        Token::Type::BYTES_LITERAL, Token::Type::TRUE, Token::Type::FALSE,          \
        Token::Type::NULL_LITERAL

#define CommandTokens                                                               \
    Token::Type::SELECT, Token::Type::INSERT, Token::Type::DELETE,                  \
        Token::Type::UPDATE, Token::Type::DROP, Token::Type::CREATE

#define DatatypeTokens                                                              \
    Token::Type::INT, Token::Type::BOOL, Token::Type::STRING, Token::Type::BYTES

#define AttributeTokens                                                             \
    Token::Type::KEY, Token::Type::UNIQUE, Token::Type::AUTOINCREMENT,              \
        Token::Type::NOT_NULL

#define KeywordTokens                                                               \
    Token::Type::TABLE, Token::Type::TO, Token::Type::SET, Token::Type::WHERE,      \
        Token::Type::FROM, AttributeTokens

#define PunctTokens                                                                 \
    Token::Type::NOT, Token::Type::COMMA, Token::Type::DOT, Token::Type::COLON,     \
        Token::Type::SEMICOLON, Token::Type::LEFT_PARENTHESIS,                      \
        Token::Type::RIGHT_PARENTHESIS, Token::Type::LEFT_BRACKET,                  \
        Token::Type::RIGHT_BRACKET, Token::Type::LEFT_BRACE,                        \
        Token::Type::RIGHT_BRACE, Token::Type::VLINE, Token::Type::END_OF_FILE

#define AllTokens                                                                   \
    OperatorTokens, LiteralTokens, CommandTokens, DatatypeTokens, KeywordTokens,    \
        PunctTokens

namespace memesql::internal {

const std::unordered_map<Token::Type, int> Parser::m_precedence = {
    { Token::Type::OR, 10 },      { Token::Type::AND, 20 },
    { Token::Type::NOT, 30 },     { Token::Type::XOR, 40 },
    { Token::Type::EQUAL, 50 },   { Token::Type::NOT_EQUAL, 50 },
    { Token::Type::LESS, 60 },    { Token::Type::LESS_EQUAL, 60 },
    { Token::Type::GREATER, 60 }, { Token::Type::GREATER_EQUAL, 60 },
    { Token::Type::PLUS, 70 },    { Token::Type::MINUS, 70 },
    { Token::Type::MUL, 80 },     { Token::Type::DIV, 80 },
    { Token::Type::MOD, 80 }
};

const std::unordered_map<Token::Type, BinaryOpType> Parser::m_binop_map = {
#define OPERATORS
#define MAP(type, value) { Token::Type::type, BinaryOpType::type },
#include "Parser/TokenEnum.hpp"
#undef MAP
#undef OPERATORS
};

std::shared_ptr<Command> Parser::parse(const std::string& query) {
    m_lexer    = std::make_shared<Lexer>(query);
    m_expector = std::make_unique<Expector>(m_lexer);

    Token token = m_expector->expect(CommandTokens);
    switch (token.get_type()) {
    case Token::Type::CREATE:
        return parse_create();
    case Token::Type::SELECT:
        return parse_select();
    case Token::Type::INSERT:
        return parse_insert();
    case Token::Type::UPDATE:
        return parse_update();
    case Token::Type::DELETE:
        return parse_delete();
    // case Token::Type::DROP:
    //     return parse_drop();
    default:
        throw ParserException("Unknown command", query, token.get_pos());
    }
}

std::shared_ptr<InsertCommand> Parser::parse_insert() {

    InsertCommand::InsertionType insertion;

    m_expector->expect(Token::Type::LEFT_PARENTHESIS);

    Token token = m_expector->peek_expect(Token::Type::COMMA, LiteralTokens,
                                          Token::Type::IDENTIFIER);

    switch (token.get_type()) {
    case Token::Type::IDENTIFIER:
        insertion = parse_map();
        break;
    default:
        insertion = parse_vector();
    }

    m_expector->expect(Token::Type::RIGHT_PARENTHESIS);
    m_expector->expect(Token::Type::TO);
    token                  = m_expector->expect(Token::Type::IDENTIFIER);
    std::string table_name = token.get_value<std::string>();
    m_expector->expect(Token::Type::SEMICOLON, Token::Type::END_OF_FILE);

    return std::make_shared<InsertCommand>(insertion, table_name);
}

InsertCommand::Map Parser::parse_map() {
    InsertCommand::Map map;

    Token token;

    while (true) {
        token = m_expector->expect(Token::Type::IDENTIFIER);

        std::string column_name = token.get_value<std::string>();

        m_expector->expect(Token::Type::EQUAL);

        map[column_name] = parse_expression();

        token = m_expector->peek_expect(
            Token::Type::COMMA, Token::Type::RIGHT_PARENTHESIS, Token::Type::WHERE);
        if (token.get_type() == Token::Type::COMMA) {
            m_expector->expect(Token::Type::COMMA);
        } else {
            break;
        }
    }

    return map;
}

InsertCommand::Vector Parser::parse_vector() {
    InsertCommand::Vector vector;

    Token token = m_expector->peek_expect(Token::Type::COMMA, LiteralTokens,
                                          Token::Type::RIGHT_PARENTHESIS);

    while (token.get_type() != Token::Type::RIGHT_PARENTHESIS) {
        token = m_expector->peek_expect(Token::Type::COMMA, LiteralTokens);

        if (token.get_type() == Token::Type::COMMA) {
            m_expector->expect(Token::Type::COMMA);
            vector.push_back(std::make_shared<ConstantExpr>());
            continue;
        }

        vector.push_back(parse_expression());
        token = m_expector->peek_expect(Token::Type::COMMA,
                                        Token::Type::RIGHT_PARENTHESIS);
        if (token.get_type() == Token::Type::COMMA) {
            m_expector->expect(Token::Type::COMMA);
        }
    }

    return vector;
}

std::shared_ptr<CreateCommand> Parser::parse_create() {
    m_expector->expect(Token::Type::TABLE);

    Token token            = m_expector->expect(Token::Type::IDENTIFIER);
    std::string table_name = token.get_value<std::string>();

    Table::Header header = parse_header();

    m_expector->expect(Token::Type::SEMICOLON, Token::Type::END_OF_FILE);

    return std::make_shared<CreateCommand>(table_name, header);
}

Table::Header Parser::parse_header() {
    Table::Header header;

    Token token = m_expector->expect(Token::Type::LEFT_PARENTHESIS);
    while (token.get_type() != Token::Type::RIGHT_PARENTHESIS) {
        token = m_expector->expect(Token::Type::IDENTIFIER, Token::Type::LEFT_BRACE);
        Table::Header::Column column;

        if (token.get_type() == Token::Type::LEFT_BRACE) {
            column.attributes = parse_attributes();
            token             = m_expector->expect(Token::Type::IDENTIFIER);
        }

        std::string name = token.get_value<std::string>();

        if (header.columns.contains(name)) {
            throw ParserException("duplicate column name", m_lexer->get_input(),
                                  token.get_pos());
        }

        m_expector->expect(Token::Type::COLON);
        token = m_expector->expect(DatatypeTokens);
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
            m_expector->expect(Token::Type::LEFT_BRACKET);
            token         = m_expector->expect(Token::Type::NUMBER_LITERAL);
            column.length = token.get_value<int>();
            m_expector->expect(Token::Type::RIGHT_BRACKET);
        }

        token = m_expector->expect(Token::Type::COMMA, Token::Type::EQUAL,
                                   Token::Type::RIGHT_PARENTHESIS);

        if (token.get_type() == Token::Type::EQUAL) {
            token = m_expector->expect(LiteralTokens);

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

            token = m_expector->expect(Token::Type::COMMA,
                                       Token::Type::RIGHT_PARENTHESIS);
        }

        column.index         = header.columns.size();
        header.columns[name] = column;
    }
    return header;
}

ColumnFields::Attributes Parser::parse_attributes() {
    ColumnFields::Attributes attributes;

    Token token = m_expector->expect(AttributeTokens, Token::Type::RIGHT_BRACE);

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
        token = m_expector->expect(Token::Type::COMMA, Token::Type::RIGHT_BRACE);
        if (token.get_type() == Token::Type::COMMA) {
            token = m_expector->expect(AttributeTokens);
        }
    }

    return attributes;
}

std::shared_ptr<SelectCommand> Parser::parse_select() {
    std::vector<SelectCommand::Column> columns;

    Token token;

    bool all_blank = true;

    while (token.get_type() != Token::Type::FROM) {
        columns.push_back(parse_column());
        if (!columns.back().table_name.empty()) {
            all_blank = false;
        }
        token = m_expector->expect(Token::Type::COMMA, Token::Type::FROM);
    }

    token = m_expector->expect(Token::Type::IDENTIFIER);

    std::string table_name = token.get_value<std::string>();

    if (all_blank) {
        for (auto&& [column_name, _] : columns) {
            column_name = table_name;
        }
    }

    token = m_expector->expect(Token::Type::WHERE, Token::Type::END_OF_FILE,
                               Token::Type::SEMICOLON);

    std::shared_ptr<Expression> expr;
    if (token.get_type() == Token::Type::WHERE) {
        expr = parse_expression();
        m_expector->expect(Token::Type::END_OF_FILE, Token::Type::SEMICOLON);
    } else {
        expr = std::make_shared<ConstantExpr>(true);
    }
    return std::make_shared<SelectCommand>(columns, table_name, expr);
}

SelectCommand::Column Parser::parse_column() {
    Token token = m_expector->expect(Token::Type::IDENTIFIER, Token::Type::MUL);

    std::string first_field, second_field;

    if (token.get_type() == Token::Type::IDENTIFIER) {
        first_field = token.get_value<std::string>();
    } else {
        first_field = "*";
    }

    token = m_expector->peek_expect(Token::Type::DOT, Token::Type::COMMA,
                                    Token::Type::FROM);

    if (token.get_type() == Token::Type::DOT) {
        m_expector->expect(Token::Type::DOT);
        token = m_expector->expect(Token::Type::IDENTIFIER, Token::Type::MUL);

        if (token.get_type() == Token::Type::IDENTIFIER) {
            second_field = token.get_value<std::string>();
        } else {
            second_field = "*";
        }
    } else {
        second_field = first_field;
        first_field.clear();
    }

    return { first_field, second_field };
}

std::shared_ptr<Expression> Parser::parse_paren_expression(
    Token::Type expect_right) {
    std::shared_ptr<Expression> expr = parse_expression();
    m_expector->expect(expect_right);
    return expr;
}

std::shared_ptr<Expression> Parser::parse_value() {
    Token token =
        m_expector->expect(Token::Type::MINUS, Token::Type::NOT, LiteralTokens);

    switch (token.get_type()) {
    case Token::Type::MINUS:
        return std::make_shared<UnaryOpExpression>(UnaryOpType::MINUS,
                                                   parse_expression());
    case Token::Type::NOT:
        return std::make_shared<UnaryOpExpression>(UnaryOpType::NOT,
                                                   parse_expression());
    default:
        return std::make_shared<ConstantExpr>(token.get_cell());
    }
}

std::shared_ptr<Expression> Parser::parse_primary() {
    Token token =
        m_expector->peek_expect(LiteralTokens, Token::Type::LEFT_PARENTHESIS,
                                Token::Type::VLINE, Token::Type::IDENTIFIER);

    switch (token.get_type()) {
    case Token::Type::LEFT_PARENTHESIS:
        m_expector->expect(Token::Type::LEFT_PARENTHESIS);
        return parse_paren_expression(Token::Type::RIGHT_PARENTHESIS);
    case Token::Type::VLINE:
        m_expector->expect(Token::Type::VLINE);
        return std::make_shared<UnaryOpExpression>(
            UnaryOpType::LENGTH, parse_paren_expression(Token::Type::VLINE));
    case Token::Type::IDENTIFIER:
        m_expector->expect(Token::Type::IDENTIFIER);
        return std::make_shared<ColumnExpr>(token.get_value<String>());
    default:
        return parse_value();
    }
}

std::shared_ptr<Expression> Parser::parse_binop_right(
    int prev_prec, std::shared_ptr<Expression> left) {
    while (1) {
        Token check_next = m_expector->peek_expect(AllTokens);

        int cur_prec = get_precedence(check_next.get_type());

        if (cur_prec < prev_prec) {
            return left;
        }

        Token bin_op_token = m_expector->expect(OperatorTokens);

        auto right = parse_primary();

        check_next    = m_expector->peek_expect(AllTokens);
        int next_prec = get_precedence(check_next.get_type());

        if (cur_prec < next_prec) {
            right = parse_binop_right(prev_prec + 1, right);
        }

        left = std::make_shared<BinaryOpExpr>(
            m_binop_map.at(bin_op_token.get_type()), left, right);
    }
}

int Parser::get_precedence(Token::Type type) const {
    if (m_precedence.contains(type)) {
        return m_precedence.at(type);
    }
    return -1;
}

std::shared_ptr<Expression> Parser::parse_expression() {
    auto left = parse_primary();

    auto ret = parse_binop_right(0, left);
    return ret;
}

std::shared_ptr<DeleteCommand> Parser::parse_delete() {
    Token token            = m_expector->expect(Token::Type::IDENTIFIER);
    std::string table_name = token.get_value<std::string>();

    m_expector->expect(Token::Type::WHERE);
    std::shared_ptr<Expression> expr = parse_expression();

    m_expector->expect(Token::Type::END_OF_FILE, Token::Type::SEMICOLON);

    return std::make_shared<DeleteCommand>(table_name, expr);
}

std::shared_ptr<UpdateCommand> Parser::parse_update() {
    Token token            = m_expector->expect(Token::Type::IDENTIFIER);
    std::string table_name = token.get_value<std::string>();

    m_expector->expect(Token::Type::SET);
    InsertCommand::Map update_map = parse_map();

    m_expector->expect(Token::Type::WHERE);
    auto&& condition = parse_expression();

    m_expector->expect(Token::Type::END_OF_FILE, Token::Type::SEMICOLON);

    return std::make_shared<UpdateCommand>(update_map, table_name, condition);
}

} // namespace memesql