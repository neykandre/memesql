#pragma once

#include "Token.hpp"
#include <unordered_map>

namespace memesql {
class Lexer {
  public:
    explicit Lexer(const std::string& input);

    Token get_next_token();
    size_t get_pos() const;
    const std::string& get_token_name(const Token::Type&) const;
    const std::string& get_input() const;

  private:
    std::string m_input;
    size_t m_pos;

    void skip_space();

    std::string get_word();
    std::string get_string_literal();
    Int get_number_literal();
    std::string get_punct();

    std::string to_lower(const std::string& str);

    static const std::unordered_map<std::string, Token::Type> m_tokens;
};
} // namespace memesql