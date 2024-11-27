#include "DataBase.hpp"
#include "Exceptions.hpp"
#include "Parser/Lexer.hpp"
#include <array>
#include <ios>
#include <iostream>
#include <memory>
#include <vector>
using namespace memesql;

int main() {
    // try {
    DataBase db;
    db.execute("create table users ({key, autoincrement} id :"
               "int32, {} login: string[32], password_hash: bytes[8], is_admin:"
               "bool = false)");

    db.execute("insert (,\"vasya\", 0xdeadbeefdeadbeef) to users");

    db.execute("insert (login = \"vasya\", password_hash = "
               "0xdeadbeefdeadbeef) to users");

    db.execute("insert (,\"admin\", 0x0000000000000000, true) to users");

    db.execute("insert (\n"
               "is_admin = true,\n"
               "login = \"admin\",\n"
               "password_hash = 0x0000000000000000\n"
               ") to users");

    auto response = db.execute(
        "select id, login, password_hash, is_admin from users where (id <= 10) && |login| > 5");
    std::cout << std::hex;
    for (auto&& row : response) {
        std::cout << row.get<Int>("id") << " " << row.get<String>("login") << " "
                  << row.get<Bytes>("password_hash") << " "
                  << row.get<Bool>("is_admin") << std::endl;
    }

    // } catch (const DBException& e) {
    //     std::cout << e.what() << "\n";
    // }

    return 0;
}