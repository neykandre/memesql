#include "DataBase.hpp"
#include "Parser/Lexer.hpp"
#include <ios>
#include <iostream>
#include <memory>
#include <vector>
using namespace memesql;

int main() {

    DataBase db;
    db.execute(
        "create table users ({key, autoincrement} id :"
        "int32, {} login: string[32], password_hash: bytes[8], is_admin:"
        "bool = false)");

    auto response = db.execute("insert (,\"vasya\", 0xdeadbeefdeadbeef) to users");

    std::cout << std::hex;

    for (auto&& row : response) {
        auto id            = row.get<Int>("id");
        auto login         = row.get<String>("login");
        auto password_hash = row.get<Bytes>("password_hash");
        auto is_admin      = row.get<Bool>("is_admin");
        std::cout << id << " " << login << " " << password_hash << " " << is_admin
                  << "\n";
    }

    response = db.execute(
        "insert (login = \"vasya\", password_hash = 0xdeadbeefdeadbeef) to users");

    for (auto&& row : response) {
        auto id            = row.get<Int>("id");
        auto login         = row.get<String>("login");
        auto password_hash = row.get<Bytes>("password_hash");
        auto is_admin      = row.get<Bool>("is_admin");
        std::cout << id << " " << login << " " << password_hash << " " << is_admin
                  << "\n";
    }

    response = db.execute("insert (,\"admin\", 0x0000000000000000, true) to users");

    for (auto&& row : response) {
        auto id            = row.get<Int>("id");
        auto login         = row.get<String>("login");
        auto password_hash = row.get<Bytes>("password_hash");
        auto is_admin      = row.get<Bool>("is_admin");
        std::cout << id << " " << login << " " << password_hash << " " << is_admin
                  << "\n";
    }

    response = db.execute("insert (\n"
                          "is_admin = true,\n"
                          "login = \"admin\",\n"
                          "password_hash = 0x0000000000000000\n"
                          ") to users");

    for (auto&& row : response) {
        auto id            = row.get<Int>("id");
        auto login         = row.get<String>("login");
        auto password_hash = row.get<Bytes>("password_hash");
        auto is_admin      = row.get<Bool>("is_admin");
        std::cout << id << " " << login << " " << password_hash << " " << is_admin
                  << "\n";
    }

    // Lexer lexer("insert (,\"vasya\", 0xdeadbeefdeadbeef) to users");
    // auto token = lexer.get_next_token();
    // while (token.get_type() != Token::Type::END_OF_FILE) {
    //     std::cout << Cell{token.get_cell()} << " ";
    //     token = lexer.get_next_token();
    // }

    return 0;
}