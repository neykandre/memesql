#include "../include/DataBase.hpp"
#include "../include/Exceptions.hpp"
#include "gtest/gtest.h"

using namespace memesql;

DataBase db;

TEST(SelectTest, TrueSelect) {
    auto response = db.execute(
        "select id, login, password_hash, is_admin from users where true");
    std::cout << std::hex;
    for (auto&& row : response) {
        std::cout << row.get<Int>("id") << " " << row.get<String>("login") << " "
                  << row.get<Bytes>("password_hash") << " "
                  << row.get<Bool>("is_admin") << std::endl;
    }
}

TEST(SelectTest, ParenTrueSelect) {
    auto response = db.execute(
        "select id, login, password_hash, is_admin from users where (true)");
    std::cout << std::hex;
    for (auto&& row : response) {
        std::cout << row.get<Int>("id") << " " << row.get<String>("login") << " "
                  << row.get<Bytes>("password_hash") << " "
                  << row.get<Bool>("is_admin") << std::endl;
    }
}

int main(int argc, char** argv) {
    db.execute(
        "create table users ({key, autoincrement} id :"
        "int32, {unique} login: string[32], password_hash: bytes[8], is_admin:"
        "bool = false)");

    db.execute("insert (,\"vasya\", 0xdeadbeefdeadbeef) to users");

    db.execute("insert (login = \"petya\", password_hash = "
               "0xdeadbeefdeadbeef) to users");

    db.execute("insert (,\"admin\", 0x0000000000000000, true) to users");

    db.execute("insert (\n"
               "is_admin = true,\n"
               "login = \"moderator\",\n"
               "password_hash = 0x0000000000000000\n"
               ") to users");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}