#include "DataBase.hpp"
#include <iostream>
using namespace memesql;

int main() {

    DataBase db;
    auto response = db.execute(
        "create table users ({key, autoincrement} id :"
        "int32, {unique} login: string[32], password_hash: bytes[8], is_admin:"
        "bool = false)");

    if (response.is_ok()) {
        std::cout << "Table created" << std::endl;
    } else {
        std::cout << "Table creation failed" << std::endl;
    }

    return 0;
}