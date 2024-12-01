#pragma once

#include "Parser/Lexer.hpp"
#include <cstring>
#include <memory>

namespace memesql::internal {
class Expector {
  public:
    explicit Expector(std::shared_ptr<Lexer>);

    std::string append_type_name(Token::Type);
    bool check_expect(Token) const;

    template <typename... Types>
    Token peek_expect(Types... types) {
        return expect_with_method<&Lexer::peek_next_token>(types...);
    }

    template <typename... Types>
    Token expect(Types... types) {
        return expect_with_method<&Lexer::get_next_token>(types...);
    }

  private:
    std::shared_ptr<Lexer> m_lexer;

    template <Token (Lexer::*method)(), typename... Types>
    Token expect_with_method(Types... types) {
        static_assert((std::is_same_v<Types, Token::Type> && ...),
                      "All arguments to expect must be of type Token::Type");
        Token token = (m_lexer.get()->*method)();
        if (!((token.get_type() == types) || ...)) {

            std::string message = "expected one of ";

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
};
} // namespace memesql