#include "Parser/Expector.hpp"
#include "Parser/Token.hpp"

namespace memesql::internal {
Expector::Expector(std::shared_ptr<Lexer> lexer)
    : m_lexer(lexer) {
}

std::string Expector::append_type_name(Token::Type type) {
    if (type > Token::Type::KEYWORDS_START && type < Token::Type::KEYWORDS_END) {
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
    } else if (type > Token::Type::DATATYPES_START &&
               type < Token::Type::DATATYPES_END) {
        return "datatype '" + m_lexer->get_token_name(type) + "'";
    } else if (type > Token::Type::LITERALS_START &&
               type < Token::Type::LITERALS_END) {
        return "literal '" + m_lexer->get_token_name(type) + "'";
    } else {
        return std::string{ "unknown token type" };
    }
}

} // namespace memesql