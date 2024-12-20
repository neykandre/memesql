#include "DataBase.hpp"
#include "Exceptions.hpp"
#include "FixedResponseIterator.hpp"
#include "Parser/Lexer.hpp"
#include <array>
#include <ios>
#include <iostream>
#include <memory>
#include <vector>
using namespace memesql;

int main() {
    std::cout << std::hex;
    // try {
    DataBase db;
    db.execute(
        "create table users ({key, autoincrement} id :"
        "int32, {unique} login: string[32], password_hash: bytes[8], is_admin:"
        "bool = false)");

    db.execute("insert (,\"vasya\", 0xdeadbeefdeadbeef) to users");

    db.execute("insert (login = \"petya\" + \"123\", password_hash = "
               "0xdeadbeefdeadbeef) to users");

    db.execute("insert (,\"admin\", 0x0000000000000000, true) to users");

    db.execute("insert (\n"
               "is_admin = true,\n"
               "login = \"moder\",\n"
               "password_hash = 0x0000000000000000\n"
               ") to users");

    auto response11 = db.execute("select * from users");
    for(auto&& [id, login, pass, is_admin] : response11.get<4>()) {
        std::cout << id << " " << login << " " << pass << " " << is_admin << std::endl;
    }
    std::cout << std::endl;

    db.execute("delete users where id = 0");

    auto response0 = db.execute("update users set login = login + \"_admin\" where (is_admin)");
    for(auto&& [id, login, pass, is_admin] : response0.get<4>()) {
        std::cout << id << " " << login << " " << pass << " " << is_admin << std::endl;
    }
    // std::cout << std::endl;

    auto response = db.execute("select * from users");

    // response.get<4>();

    for(auto&& [id, login, pass, _] : response.get<4>()) {
        std::cout << id << " " << login << " " << pass << " " << std::endl;
    }

    return 0;
}
