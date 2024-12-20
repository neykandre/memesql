#include "Parser/Lexer.hpp"
#include "Exceptions.hpp"
#include <algorithm>
#include <cctype>

namespace memesql::internal {
const std::unordered_map<std::string, Token::Type> Lexer::m_tokens = {
#define MAPPING
#include "Parser/TokenEnum.hpp"
#undef MAPPING
};

Lexer::Lexer(const std::string& input)
    : m_input(input),
      m_pos(0) {
}

size_t Lexer::get_pos() const {
    return m_pos;
}

void Lexer::set_pos(size_t pos) {
    m_pos = pos;
}

void Lexer::skip_space() {
    while (m_pos < m_input.size() && std::isspace(m_input[m_pos])) {
        ++m_pos;
    }
}

const std::string& Lexer::get_token_name(const Token::Type& type) const {
    for (const auto& [key, value] : m_tokens) {
        if (value == type) {
            return key;
        }
    }

    throw LexerException("Unknown token type", "", 0);
}

Token Lexer::peek_next_token() {
    size_t pos = m_pos;
    Token token = get_next_token();
    set_pos(pos);
    return token;
}

Token Lexer::get_next_token() {
    skip_space();

    size_t start = m_pos;

    if (m_pos == m_input.size()) {
        return { Token::Type::END_OF_FILE, start };
    }

    if (m_input[m_pos] == '"') {
        std::string str = get_string_literal();

        return { Token::Type::STRING_LITERAL, str, start };
    }

    if (m_pos + 2 < m_input.size() && m_input.substr(m_pos, 2) == "0x") {
        std::string str = get_word();

        return { Token::Type::BYTES_LITERAL, Bytes{ str }, start };
    }

    if (std::isdigit(m_input[m_pos])) {
        Int number = get_number_literal();

        return { Token::Type::NUMBER_LITERAL, number, start };
    }

    if (std::isalpha(m_input[m_pos]) || m_input[m_pos] == '_') {
        std::string word = get_word();

        if (m_tokens.contains(to_lower(word))) {
            if (word == "true") {
                return { Token::Type::TRUE, true, start };
            }
            if (word == "false") {
                return { Token::Type::FALSE, false, start };
            }
            if (word == "null") {
                return { Token::Type::NULL_LITERAL, Null{}, start };
            }
            return { m_tokens.at(to_lower(word)), word, start };
        }

        return { Token::Type::IDENTIFIER, word, start };
    }

    if (std::ispunct(m_input[m_pos])) {
        std::string punct = get_punct();
        while (!m_tokens.contains(punct)) {
            m_pos--;
            punct.pop_back();
        }

        if (punct.empty()) {
            throw LexerException("Unknown token", m_input, start);
        }

        return { m_tokens.at(punct), punct, start };
    }

    throw LexerException("Unknown token" + std::string(1, m_input[m_pos]), m_input,
                         start);
}

const std::string& Lexer::get_input() const {
    return m_input;
}

std::string Lexer::to_lower(const std::string& str) {
    std::string result;
    result.resize(str.size());
    std::transform(str.begin(), str.end(), result.begin(), tolower);
    return result;
}

std::string Lexer::get_word() {
    size_t start = m_pos;
    while (m_pos < m_input.size() &&
           (std::isalnum(m_input[m_pos]) || m_input[m_pos] == '_')) {
        ++m_pos;
    }
    return m_input.substr(start, m_pos - start);
}

std::string Lexer::get_string_literal() {
    size_t start = ++m_pos;
    while (m_pos < m_input.size() && m_input[m_pos] != '"') {
        ++m_pos;
    }

    if (m_pos == m_input.size()) {
        throw LexerException("Unterminated string literal", m_input, m_pos);
    }
    ++m_pos;

    return m_input.substr(start, m_pos - start - 1);
}

Int Lexer::get_number_literal() {
    size_t start = m_pos;
    while (m_pos < m_input.size() && std::isdigit(m_input[m_pos])) {
        ++m_pos;
    }

    if (m_pos < m_input.size() && std::isalpha(m_input[m_pos])) {
        throw LexerException("Invalid number literal", m_input, start);
    }

    return std::stoi(m_input.substr(start, m_pos - start));
}

std::string Lexer::get_punct() {
    size_t start = m_pos;
    while (m_pos < m_input.size() && std::ispunct(m_input[m_pos])) {
        ++m_pos;
    }
    return m_input.substr(start, m_pos - start);
}

} // namespace memesql