#pragma once

#include <stdexcept>

namespace memesql {

class DBException : public std::runtime_error {
  public:
    using std::runtime_error::runtime_error;
};

class CommandException : public DBException {
  public:
    using DBException::DBException;
};

class LexerException : public DBException {
  public:
    LexerException(const std::string& message, const std::string& input, size_t pos)
        : DBException("\n" + input + "\n" + std::string(pos, ' ') + "^ " + message) {
    }
};

class ParserException : public DBException {
  public:
    ParserException(const std::string& message, const std::string& input, size_t pos)
        : DBException("\n" + input + "\n" + std::string(pos, ' ') + "^ " + message) {
    }
};

class ExpressionException : public DBException {
  public:
    ExpressionException(const std::string& message, const std::string& input, size_t pos)
        : DBException("\n" + input + "\n" + std::string(pos, ' ') + "^ " + message) {
    }
};

} // namespace memesql