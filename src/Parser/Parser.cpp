#include "Parser/Parser.hpp"
#include "Definitions.hpp"
#include "Exceptions.hpp"
#include "Parser/Lexer.hpp"
#include "Parser/Token.hpp"

namespace memesql {

std::shared_ptr<Command> Parser::parse(const std::string& query) {
    m_lexer = std::make_unique<Lexer>(query);

    Token token =
        expect_one_of(Token::Type::CREATE, Token::Type::SELECT, Token::Type::INSERT,
                      Token::Type::UPDATE, Token::Type::DELETE, Token::Type::DROP);
    switch (token.get_type()) {
    case Token::Type::CREATE:
        return parse_create();
    // case Token::Type::SELECT:
    //     return parse_select();
    case Token::Type::INSERT:
        return parse_insert();
    // case Token::Type::UPDATE:
    //     return parse_update();
    // case Token::Type::DELETE:
    //     return parse_delete();
    // case Token::Type::DROP:
    //     return parse_drop();
    default:
        throw ParserException("Unknown command", query, token.get_pos());
    }
}

Token Parser::expect(Token::Type type) {
    return expect_one_of(type);
}

bool Parser::check_expect(Token token) const {
    return !(token.get_pos() == m_lexer->get_pos());
}

Token Parser::try_expect(Token::Type type) {
    return try_expect_one_of(type);
}

} // namespace memesql