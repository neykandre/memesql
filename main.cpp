#include "Parser/Parser.hpp"
using namespace memesql;

int main() {
    Parser parser;

    auto command = parser.parse("create table users ({key, autoincrement} id :"
                                "int3,"
                                "{ unique } login"
                                ": string[32], password_hash"
                                ": bytes[8], is_admin"
                                ": bool = false) ");

    // auto token = lexer.get_next_token();
    // while (token.get_type() != Token::Type::END_OF_FILE) {
    //     switch (token.get_type()) {
    //         #define ALL
    //         #define TOKEN(name) case Token::Type::name: std::cout << #name << ":
    //         "; break; #include "Parser/TokenEnum.hpp" #undef ALL #undef TOKEN
    //         default:
    //             std::cout << "Unknown token" << std::endl;
    //             break;
    //     }
    //     std::visit(
    //         [](auto&& value) {
    //             std::cout << value << std::endl;
    //         },
    //         token.get_value());
    //     token = lexer.get_next_token();
    // }
}