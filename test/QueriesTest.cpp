#include "../include/DataBase.hpp"
#include "../include/Exceptions.hpp"
#include "gtest/gtest.h"

using namespace memesql;

TEST(CreateTest, SimpleCreateTable) {
    DataBase db;
    EXPECT_NO_THROW(db.execute("create table users (id :int32, login: string[32], "
                               "password_hash: bytes[8], is_admin: bool)"));
}

TEST(CreateTest, SimpleCreateTableWithAttributes) {
    DataBase db;
    EXPECT_NO_THROW(db.execute("create table users ({key, autoincrement} id :int32, "
                               "{unique} login: string[32], "
                               "password_hash: bytes[8], is_admin: bool = false)"));
}

TEST(CreateTest, SimpleCreateTableWithDefaults) {
    DataBase db;
    EXPECT_NO_THROW(db.execute("create table users (id :int32, login: string[32], "
                               "password_hash: bytes[8], is_admin: bool = false, "
                               "is_active: bool = true)"));
}

TEST(CreateTest, SimpleCreateTableWithAttributesAndDefaults) {
    DataBase db;
    EXPECT_NO_THROW(db.execute("create table users ({key, autoincrement} id :int32, "
                               "{unique} login: string[32], "
                               "password_hash: bytes[8], is_admin: bool = false, "
                               "is_active: bool = true)"));
}

TEST(CreateTest, TwoTables) {
    DataBase db;
    EXPECT_NO_THROW(db.execute("create table users (id :int32, login: string[32], "
                               "password_hash: bytes[8], is_admin: bool = false)"));
    EXPECT_NO_THROW(db.execute("create table users2 (id :int32, login: string[32], "
                               "password_hash: bytes[8], is_admin: bool = false)"));
}

TEST(CreateTest, TwoSameTables) {
    DataBase db;
    EXPECT_NO_THROW(db.execute("create table users (id :int32, login: string[32], "
                               "password_hash: bytes[8], is_admin: bool = false)"));
    EXPECT_THROW(db.execute("create table users (id :int32, login: string[32], "
                            "password_hash: bytes[8], is_admin: bool = false)"),
                 DBException);
}

TEST(InsertTest, FullMapInsert) {
    DataBase db;
    db.execute("create table users ({key, autoincrement} id :int32, "
               "{unique} login: string[32], "
               "password_hash: bytes[8], is_admin: bool = false)");

    Response response;
    EXPECT_NO_THROW(
        response = db.execute("insert (id = 1, login = \"vasya\", password_hash = "
                              "0xdeadbeefdeadbeef, is_admin = false) to users"));
    EXPECT_THROW(db.execute("insert (id = 1, login = \"vasya\", password_hash = "
                            "0xdeadbeefdeadbeef, is_admin = false) to users"),
                 DBException);
    EXPECT_EQ((*response.begin()).get<int>("id"), 1);
    EXPECT_EQ((*response.begin()).get<std::string>("login"), "vasya");
    EXPECT_EQ((*response.begin()).get<Bytes>("password_hash"),
              Bytes{ "0xdeadbeefdeadbeef" });
    EXPECT_EQ((*response.begin()).get<bool>("is_admin"), false);

    EXPECT_NO_THROW(
        response = db.execute("insert (id = 2, login = \"petya\", password_hash = "
                              "0xdeadbeefdeadbeef, is_admin = false) to users"));
    EXPECT_THROW(db.execute("insert (id = 2, login = \"petya\", password_hash = "
                            "0xdeadbeefdeadbeef, is_admin = false) to users"),
                 DBException);
    EXPECT_EQ((*response.begin()).get<int>("id"), 2);
    EXPECT_EQ((*response.begin()).get<std::string>("login"), "petya");
    EXPECT_EQ((*response.begin()).get<Bytes>("password_hash"),
              Bytes{ "0xdeadbeefdeadbeef" });
    EXPECT_EQ((*response.begin()).get<bool>("is_admin"), false);

    EXPECT_NO_THROW(
        response = db.execute("insert (id = 3, login = \"admin\", password_hash = "
                              "0x0000000000000000, is_admin = true) to users"));
    EXPECT_THROW(db.execute("insert (id = 3, login = \"admin\", password_hash = "
                            "0x0000000000000000, is_admin = true) to users"),
                 DBException);
    EXPECT_EQ((*response.begin()).get<int>("id"), 3);
    EXPECT_EQ((*response.begin()).get<std::string>("login"), "admin");
    EXPECT_EQ((*response.begin()).get<Bytes>("password_hash"),
              Bytes{ "0x0000000000000000" });
    EXPECT_EQ((*response.begin()).get<bool>("is_admin"), true);
}

TEST(InsertTest, PartialMapInsert) {
    DataBase db;
    db.execute("create table users ({key, autoincrement} id :int32, "
               "{unique} login: string[32], "
               "password_hash: bytes[8], is_admin: bool = false)");
    Response response;
    EXPECT_NO_THROW(response =
                        db.execute("insert (login = \"vasya\", password_hash = "
                                   "0xdeadbeefdeadbeef) to users"));
    EXPECT_THROW(db.execute("insert (login = \"vasya\", password_hash = "
                            "0xdeadbeefdeadbeef) to users"),
                 DBException);
    EXPECT_EQ((*response.begin()).get<int>("id"), 0);
    EXPECT_EQ((*response.begin()).get<bool>("is_admin"), false);

    EXPECT_NO_THROW(response =
                        db.execute("insert (login = \"petya\", password_hash = "
                                   "0xdeadbeefdeadbeef) to users"));
    EXPECT_THROW(db.execute("insert (login = \"petya\", password_hash = "
                            "0xdeadbeefdeadbeef) to users"),
                 DBException);
    EXPECT_EQ((*response.begin()).get<int>("id"), 1);
    EXPECT_EQ((*response.begin()).get<bool>("is_admin"), false);

    EXPECT_NO_THROW(response =
                        db.execute("insert (login = \"admin\", password_hash = "
                                   "0x0000000000000000, is_admin = true) to users"));
    EXPECT_THROW(db.execute("insert (login = \"admin\", password_hash = "
                            "0x0000000000000000, is_admin = true) to users"),
                 DBException);
    EXPECT_EQ((*response.begin()).get<int>("id"), 2);
    EXPECT_EQ((*response.begin()).get<bool>("is_admin"), true);
}

TEST(InsertTest, FullVectorInsert) {
    DataBase db;
    db.execute("create table users ({key, autoincrement} id :int32, "
               "{unique} login: string[32], "
               "password_hash: bytes[8], is_admin: bool = false)");
    Response response;
    EXPECT_NO_THROW(response = db.execute(
                        "insert (0,\"vasya\", 0xdeadbeefdeadbeef, false) to users"));
    EXPECT_THROW(
        db.execute("insert (0,\"vasya\", 0xdeadbeefdeadbeef, false) to users"),
        DBException);
    EXPECT_EQ((*response.begin()).get<int>("id"), 0);
    EXPECT_EQ((*response.begin()).get<std::string>("login"), "vasya");
    EXPECT_EQ((*response.begin()).get<Bytes>("password_hash"),
              Bytes{ "0xdeadbeefdeadbeef" });
    EXPECT_EQ((*response.begin()).get<bool>("is_admin"), false);

    EXPECT_NO_THROW(response = db.execute(
                        "insert (1,\"petya\", 0xdeadbeefdeadbeef, false) to users"));
    EXPECT_THROW(
        db.execute("insert (1,\"petya\", 0xdeadbeefdeadbeef, false) to users"),
        DBException);
    EXPECT_EQ((*response.begin()).get<int>("id"), 1);
    EXPECT_EQ((*response.begin()).get<std::string>("login"), "petya");
    EXPECT_EQ((*response.begin()).get<Bytes>("password_hash"),
              Bytes{ "0xdeadbeefdeadbeef" });
    EXPECT_EQ((*response.begin()).get<bool>("is_admin"), false);

    EXPECT_NO_THROW(response = db.execute(
                        "insert (2,\"admin\", 0x0000000000000000, true) to users"));
    EXPECT_THROW(
        db.execute("insert (2,\"admin\", 0x0000000000000000, true) to users"),
        DBException);
    EXPECT_EQ((*response.begin()).get<int>("id"), 2);
    EXPECT_EQ((*response.begin()).get<std::string>("login"), "admin");
    EXPECT_EQ((*response.begin()).get<Bytes>("password_hash"),
              Bytes{ "0x0000000000000000" });
    EXPECT_EQ((*response.begin()).get<bool>("is_admin"), true);
}

TEST(InsertTest, PartialVectorInsert) {
    DataBase db;
    db.execute("create table users ({key, autoincrement} id :int32, "
               "{unique} login: string[32], "
               "password_hash: bytes[8], is_admin: bool = false)");
    Response response;
    EXPECT_NO_THROW(
        response = db.execute("insert (,\"vasya\", 0xdeadbeefdeadbeef) to users"));
    EXPECT_THROW(db.execute("insert (,\"vasya\", 0xdeadbeefdeadbeef) to users"),
                 DBException);
    EXPECT_EQ((*response.begin()).get<int>("id"), 0);
    EXPECT_EQ((*response.begin()).get<std::string>("login"), "vasya");
    EXPECT_EQ((*response.begin()).get<Bytes>("password_hash"),
              Bytes{ "0xdeadbeefdeadbeef" });
    EXPECT_EQ((*response.begin()).get<bool>("is_admin"), false);

    EXPECT_NO_THROW(
        response = db.execute("insert (,\"petya\", 0xdeadbeefdeadbeef) to users"));
    EXPECT_THROW(db.execute("insert (,\"petya\", 0xdeadbeefdeadbeef) to users"),
                 DBException);
    EXPECT_EQ((*response.begin()).get<int>("id"), 1);
    EXPECT_EQ((*response.begin()).get<std::string>("login"), "petya");
    EXPECT_EQ((*response.begin()).get<Bytes>("password_hash"),
              Bytes{ "0xdeadbeefdeadbeef" });
    EXPECT_EQ((*response.begin()).get<bool>("is_admin"), false);

    EXPECT_NO_THROW(response = db.execute(
                        "insert (,\"admin\", 0x0000000000000000, true) to users"));
    EXPECT_THROW(
        db.execute("insert (,\"admin\", 0x0000000000000000, true) to users"),
        DBException);
    EXPECT_EQ((*response.begin()).get<int>("id"), 2);
    EXPECT_EQ((*response.begin()).get<std::string>("login"), "admin");
    EXPECT_EQ((*response.begin()).get<Bytes>("password_hash"),
              Bytes{ "0x0000000000000000" });
    EXPECT_EQ((*response.begin()).get<bool>("is_admin"), true);
}

TEST(InsertTest, EmptyInsert) {
    DataBase db;
    db.execute(
        "create table users ({key, autoincrement} id :int32, "
        "login: string[32] = \"default\", "
        "password_hash: bytes[8] = 0x0000000000000000, is_admin: bool = false)");
    Response response;
    EXPECT_NO_THROW(response = db.execute("insert () to users"));
    EXPECT_EQ((*response.begin()).get<int>("id"), 0);
    EXPECT_EQ((*response.begin()).get<std::string>("login"), "default");
    EXPECT_EQ((*response.begin()).get<Bytes>("password_hash"),
              Bytes{ "0x0000000000000000" });
    EXPECT_EQ((*response.begin()).get<bool>("is_admin"), false);
}

TEST(InsertTest, VectorExpressionInsert) {
    DataBase db;
    db.execute("create table users ({key, autoincrement} id :int32, "
               "{unique} login: string[32], "
               "password_hash: bytes[8], is_admin: bool = false)");
    Response response;
    EXPECT_NO_THROW(response =
                        db.execute("insert (1 + 2 * 3,\"vasya\" + \"123\", "
                                   "0xdeadbeefdeadbeef, true ^^ false) to users"));
    EXPECT_EQ((*response.begin()).get<std::string>("login"), "vasya123");
    EXPECT_EQ((*response.begin()).get<int>("id"), 7);
    EXPECT_EQ((*response.begin()).get<bool>("is_admin"), true);
}

TEST(InsertTest, MapExpressionInsert) {
    DataBase db;
    db.execute("create table users ({key, autoincrement} id :int32, "
               "{unique} login: string[32], "
               "password_hash: bytes[8], is_admin: bool = false)");
    Response response;
    EXPECT_NO_THROW(
        response = db.execute("insert (id = 1 + 2 * 3, login = \"vasya\" + \"123\", "
                              "password_hash = 0xdeadbeefdeadbeef, "
                              "is_admin = true ^^ false) to users"));
    EXPECT_EQ((*response.begin()).get<std::string>("login"), "vasya123");
    EXPECT_EQ((*response.begin()).get<int>("id"), 7);
    EXPECT_EQ((*response.begin()).get<bool>("is_admin"), true);
}

TEST(SelectTest, SimpleSelect) {
    DataBase db;
    db.execute("create table users ({key, autoincrement} id :int32, "
               "{unique} login: string[32], "
               "password_hash: bytes[8], is_admin: bool = false)");
    db.execute("insert (,\"vasya\", 0xdeadbeefdeadbeef) to users");
    db.execute("insert (,\"petya\", 0xdeadbeefdeadbeef) to users");
    db.execute("insert (,\"admin\", 0x0000000000000000, true) to users");
    auto response = db.execute("select * from users");
    EXPECT_EQ(response.records_count(), 3);
    auto it = response.begin();
    EXPECT_EQ((*it).get<int>("id"), 0);
    EXPECT_EQ((*it).get<std::string>("login"), "vasya");
    EXPECT_EQ((*it).get<Bytes>("password_hash"), Bytes{ "0xdeadbeefdeadbeef" });
    EXPECT_EQ((*it).get<bool>("is_admin"), false);
    ++it;
    EXPECT_EQ((*it).get<int>("id"), 1);
    EXPECT_EQ((*it).get<std::string>("login"), "petya");
    EXPECT_EQ((*it).get<Bytes>("password_hash"), Bytes{ "0xdeadbeefdeadbeef" });
    EXPECT_EQ((*it).get<bool>("is_admin"), false);
    ++it;
    EXPECT_EQ((*it).get<int>("id"), 2);
    EXPECT_EQ((*it).get<std::string>("login"), "admin");
    EXPECT_EQ((*it).get<Bytes>("password_hash"), Bytes{ "0x0000000000000000" });
    EXPECT_EQ((*it).get<bool>("is_admin"), true);
}

TEST(SelectTest, SimpleSelectWithCondition) {
    DataBase db;
    db.execute("create table users ({key, autoincrement} id :int32, "
               "{unique} login: string[32], "
               "password_hash: bytes[8], is_admin: bool = false)");
    db.execute("insert (,\"vasya\", 0xdeadbeefdeadbeef) to users");
    db.execute("insert (,\"petya\", 0xdeadbeefdeadbeef) to users");
    db.execute("insert (,\"admin\", 0x0000000000000000, true) to users");
    auto response = db.execute("select * from users where id = 0");
    EXPECT_EQ(response.records_count(), 1);
    auto it = response.begin();
    EXPECT_EQ((*it).get<int>("id"), 0);
    EXPECT_EQ((*it).get<std::string>("login"), "vasya");
    EXPECT_EQ((*it).get<Bytes>("password_hash"), Bytes{ "0xdeadbeefdeadbeef" });
    EXPECT_EQ((*it).get<bool>("is_admin"), false);

    response = db.execute("select * from users where id = 1");
    EXPECT_EQ(response.records_count(), 1);
    it = response.begin();
    EXPECT_EQ((*it).get<int>("id"), 1);
    EXPECT_EQ((*it).get<std::string>("login"), "petya");
    EXPECT_EQ((*it).get<Bytes>("password_hash"), Bytes{ "0xdeadbeefdeadbeef" });
    EXPECT_EQ((*it).get<bool>("is_admin"), false);

    response = db.execute("select * from users where login = \"vasya\"");
    EXPECT_EQ(response.records_count(), 1);
    it = response.begin();
    EXPECT_EQ((*it).get<int>("id"), 0);
    EXPECT_EQ((*it).get<std::string>("login"), "vasya");
    EXPECT_EQ((*it).get<Bytes>("password_hash"), Bytes{ "0xdeadbeefdeadbeef" });
    EXPECT_EQ((*it).get<bool>("is_admin"), false);

    response = db.execute("select * from users where is_admin");
    EXPECT_EQ(response.records_count(), 1);
    it = response.begin();
    EXPECT_EQ((*it).get<int>("id"), 2);
    EXPECT_EQ((*it).get<std::string>("login"), "admin");
    EXPECT_EQ((*it).get<Bytes>("password_hash"), Bytes{ "0x0000000000000000" });
    EXPECT_EQ((*it).get<bool>("is_admin"), true);

    response = db.execute("select * from users where is_admin = true");
    EXPECT_EQ(response.records_count(), 1);
    it = response.begin();
    EXPECT_EQ((*it).get<int>("id"), 2);
    EXPECT_EQ((*it).get<std::string>("login"), "admin");
    EXPECT_EQ((*it).get<Bytes>("password_hash"), Bytes{ "0x0000000000000000" });
    EXPECT_EQ((*it).get<bool>("is_admin"), true);

    response = db.execute("select * from users where !is_admin");
    EXPECT_EQ(response.records_count(), 2);
    it = response.begin();
    EXPECT_EQ((*it).get<int>("id"), 0);
    EXPECT_EQ((*it).get<std::string>("login"), "vasya");
    EXPECT_EQ((*it).get<Bytes>("password_hash"), Bytes{ "0xdeadbeefdeadbeef" });
    EXPECT_EQ((*it).get<bool>("is_admin"), false);
    ++it;
    EXPECT_EQ((*it).get<int>("id"), 1);
    EXPECT_EQ((*it).get<std::string>("login"), "petya");
    EXPECT_EQ((*it).get<Bytes>("password_hash"), Bytes{ "0xdeadbeefdeadbeef" });
    EXPECT_EQ((*it).get<bool>("is_admin"), false);

    response = db.execute("select * from users where is_admin = false");
    EXPECT_EQ(response.records_count(), 2);
    it = response.begin();
    EXPECT_EQ((*it).get<int>("id"), 0);
    EXPECT_EQ((*it).get<std::string>("login"), "vasya");
    EXPECT_EQ((*it).get<Bytes>("password_hash"), Bytes{ "0xdeadbeefdeadbeef" });
    EXPECT_EQ((*it).get<bool>("is_admin"), false);
    ++it;
    EXPECT_EQ((*it).get<int>("id"), 1);
    EXPECT_EQ((*it).get<std::string>("login"), "petya");
    EXPECT_EQ((*it).get<Bytes>("password_hash"), Bytes{ "0xdeadbeefdeadbeef" });
    EXPECT_EQ((*it).get<bool>("is_admin"), false);
}

TEST(SelectTest, PartialSelect) {
    DataBase db;
    db.execute("create table users ({key, autoincrement} id :int32, "
               "{unique} login: string[32], "
               "password_hash: bytes[8], is_admin: bool = false)");
    EXPECT_NO_THROW(db.execute("insert (,\"vasya\", 0xdeadbeefdeadbeef) to users"));
    EXPECT_NO_THROW(db.execute("insert (,\"petya\", 0xdeadbeefdeadbeef) to users"));
    EXPECT_NO_THROW(db.execute("insert (,\"admin\", 0x0000000000000000, true) to users"));    

    Response response;
    EXPECT_NO_THROW(response = db.execute("select id from users"));
    EXPECT_EQ(response.records_count(), 3);
    auto it = response.begin();
    EXPECT_EQ((*it).get<int>("id"), 0);
    ++it;
    EXPECT_EQ((*it).get<int>("id"), 1);
    ++it;
    EXPECT_EQ((*it).get<int>("id"), 2);

    EXPECT_NO_THROW(response = db.execute("select id, login from users"));
    EXPECT_EQ(response.records_count(), 3);
    it = response.begin();
    EXPECT_EQ((*it).get<int>("id"), 0);
    EXPECT_EQ((*it).get<std::string>("login"), "vasya");
    ++it;
    EXPECT_EQ((*it).get<int>("id"), 1);
    EXPECT_EQ((*it).get<std::string>("login"), "petya");
    ++it;
    EXPECT_EQ((*it).get<int>("id"), 2);
    EXPECT_EQ((*it).get<std::string>("login"), "admin");

    EXPECT_NO_THROW(response = db.execute("select id, login, is_admin from users"));
    EXPECT_EQ(response.records_count(), 3);
    it = response.begin();
    EXPECT_EQ((*it).get<int>("id"), 0);
    EXPECT_EQ((*it).get<std::string>("login"), "vasya");
    EXPECT_EQ((*it).get<bool>("is_admin"), false);
    ++it;
    EXPECT_EQ((*it).get<int>("id"), 1);
    EXPECT_EQ((*it).get<std::string>("login"), "petya");
    EXPECT_EQ((*it).get<bool>("is_admin"), false);
    ++it;
    EXPECT_EQ((*it).get<int>("id"), 2);
    EXPECT_EQ((*it).get<std::string>("login"), "admin");
    EXPECT_EQ((*it).get<bool>("is_admin"), true);
}

TEST(SelectTest, EmptySelect) {
    DataBase db;
    db.execute("create table users ({key, autoincrement} id :int32, "
               "{unique} login: string[32], "
               "password_hash: bytes[8], is_admin: bool = false)");
    EXPECT_NO_THROW(db.execute("insert (,\"vasya\", 0xdeadbeefdeadbeef) to users"));
    EXPECT_NO_THROW(db.execute("insert (,\"petya\", 0xdeadbeefdeadbeef) to users"));
    EXPECT_NO_THROW(db.execute("insert (,\"admin\", 0x0000000000000000, true) to users"));

    Response response;
    EXPECT_NO_THROW(response = db.execute("select * from users where id = 100"));
    EXPECT_EQ(response.records_count(), 0);

    EXPECT_THROW(db.execute("select * from unknown where true"), DBException);
}

TEST(DeleteTest, SimpleDelete) {
    DataBase db;
    db.execute("create table users ({key, autoincrement} id :int32, "
               "{unique} login: string[32], "
               "password_hash: bytes[8], is_admin: bool = false)");
    EXPECT_NO_THROW(db.execute("insert (,\"vasya\", 0xdeadbeefdeadbeef) to users"));
    EXPECT_NO_THROW(db.execute("insert (,\"petya\", 0xdeadbeefdeadbeef) to users"));
    EXPECT_NO_THROW(db.execute("insert (,\"admin\", 0x0000000000000000, true) to users"));
    EXPECT_NO_THROW(db.execute("delete users where id = 0"));
    
    Response response;
    EXPECT_NO_THROW(response = db.execute("select * from users"));
    EXPECT_EQ(response.records_count(), 2);
}

TEST(UpdateTest, SimpleUpdate) {
    DataBase db;
    db.execute("create table users ({key, autoincrement} id :int32, "
               "{unique} login: string[32], "
               "password_hash: bytes[8], is_admin: bool = false)");
    EXPECT_NO_THROW(db.execute("insert (,\"vasya\", 0xdeadbeefdeadbeef) to users"));
    EXPECT_NO_THROW(db.execute("insert (,\"petya\", 0xdeadbeefdeadbeef) to users"));
    EXPECT_NO_THROW(db.execute("insert (,\"admin\", 0x0000000000000000, true) to users"));
    EXPECT_NO_THROW(db.execute("update users set is_admin = true where id = 0"));
    Response response;
    EXPECT_NO_THROW(response = db.execute("select * from users where id = 0"));
    EXPECT_EQ(response.records_count(), 1);
    EXPECT_EQ((*response.begin()).get<bool>("is_admin"), true);
}

TEST(UpdateTest, FullUpdate) {
    DataBase db;
    db.execute("create table users ({key, autoincrement} id :int32, "
               "{unique} login: string[32], "
               "password_hash: bytes[8], is_admin: bool = false)");
    EXPECT_NO_THROW(db.execute("insert (,\"vasya\", 0xdeadbeefdeadbeef) to users"));
    EXPECT_NO_THROW(db.execute("insert (,\"petya\", 0xdeadbeefdeadbeef) to users"));
    EXPECT_NO_THROW(db.execute("insert (,\"admin\", 0x0000000000000000, true) to users"));
    EXPECT_NO_THROW(db.execute("update users set is_admin = true where true"));
    Response response;
    EXPECT_NO_THROW(response = db.execute("select * from users"));
    EXPECT_EQ(response.records_count(), 3);
    auto it = response.begin();
    EXPECT_EQ((*it).get<bool>("is_admin"), true);
    ++it;
    EXPECT_EQ((*it).get<bool>("is_admin"), true);
    ++it;
    EXPECT_EQ((*it).get<bool>("is_admin"), true);
}

TEST(UpdateTest, EmptyUpdate) {
    DataBase db;
    db.execute("create table users ({key, autoincrement} id :int32, "
               "{unique} login: string[32], "
               "password_hash: bytes[8], is_admin: bool = false)");
    EXPECT_NO_THROW(db.execute("insert (,\"vasya\", 0xdeadbeefdeadbeef) to users"));
    EXPECT_NO_THROW(db.execute("insert (,\"petya\", 0xdeadbeefdeadbeef) to users"));
    EXPECT_NO_THROW(db.execute("insert (,\"admin\", 0x0000000000000000, true) to users"));
    EXPECT_NO_THROW(db.execute("update users set is_admin = true where id = 100"));
    Response response;
    EXPECT_NO_THROW(response = db.execute("select * from users"));
    EXPECT_EQ(response.records_count(), 3);
    auto it = response.begin();
    EXPECT_EQ((*it).get<bool>("is_admin"), false);
    ++it;
    EXPECT_EQ((*it).get<bool>("is_admin"), false);
    ++it;
    EXPECT_EQ((*it).get<bool>("is_admin"), true);
}

TEST(UpdateTest, UpdateWithExpression) {
    DataBase db;
    db.execute("create table users ({key, autoincrement} id :int32, "
               "{unique} login: string[32], "
               "password_hash: bytes[8], is_admin: bool = false)");
    EXPECT_NO_THROW(db.execute("insert (,\"vasya\", 0xdeadbeefdeadbeef) to users"));
    EXPECT_NO_THROW(db.execute("insert (,\"petya\", 0xdeadbeefdeadbeef) to users"));
    EXPECT_NO_THROW(db.execute("insert (,\"admin\", 0x0000000000000000, true) to users"));
    EXPECT_NO_THROW(db.execute("update users set is_admin = (login = \"vasya\") where true"));
    Response response;
    EXPECT_NO_THROW(response = db.execute("select * from users where login = \"vasya\""));
    EXPECT_EQ(response.records_count(), 1);
    EXPECT_EQ((*response.begin()).get<bool>("is_admin"), true);
}