#include "gtest/gtest.h"
#include "../include/Table.hpp"

TEST(TableTest, Constructor) {
    memesql::Table::Header header;
    header.columns["column1"].type = memesql::ColumnFields::DataTypes::INT;
    header.columns["column2"].type = memesql::ColumnFields::DataTypes::STRING;

    memesql::Table table(header);
    ASSERT_EQ(table.get_header().columns.size(), 2);
}

TEST(TableTest, GetHeader) {
    memesql::Table::Header header;
    header.columns["column1"].type = memesql::ColumnFields::DataTypes::INT;
    header.columns["column2"].type = memesql::ColumnFields::DataTypes::STRING;

    memesql::Table table(header);
    const auto& tableHeader = table.get_header();
    ASSERT_EQ(tableHeader.columns.size(), 2);
    ASSERT_EQ(tableHeader.columns.at("column1").type, memesql::ColumnFields::DataTypes::INT);
    ASSERT_EQ(tableHeader.columns.at("column2").type, memesql::ColumnFields::DataTypes::STRING);
}

TEST(TableTest, GetAllRecords) {
    memesql::Table::Header header;
    header.columns["column1"].type = memesql::ColumnFields::DataTypes::INT;
    header.columns["column1"].index = 0;
    header.columns["column2"].type = memesql::ColumnFields::DataTypes::STRING;
    header.columns["column2"].index = 1;

    memesql::Table table(header);
    table.create_record({{"column1", memesql::Cell(1)}, {"column2", memesql::Cell("hello")}});
    table.create_record({{"column1", memesql::Cell(2)}, {"column2", memesql::Cell("world")}});

    const auto& records = table.get_all_records();
    ASSERT_EQ(records.size(), 2);
}

TEST(TableTest, GetRecord) {
    memesql::Table::Header header;
    header.columns["column1"].type = memesql::ColumnFields::DataTypes::INT;
    header.columns["column1"].index = 0;
    header.columns["column2"].type = memesql::ColumnFields::DataTypes::STRING;
    header.columns["column2"].index = 1;

    memesql::Table table(header);
    table.create_record({{"column1", memesql::Cell(1)}, {"column2", memesql::Cell("hello")}});
    table.create_record({{"column1", memesql::Cell(2)}, {"column2", memesql::Cell("world")}});

    const auto& record = table.get_record(0);
    ASSERT_NE(record, nullptr);
    ASSERT_EQ(record->get_cell(header.columns.at("column1").index).get<int>(), 1);
    ASSERT_EQ(record->get_cell(header.columns.at("column2").index).get<std::string>(), "hello");
}

TEST(TableTest, GetRecordOutOfBounds) {
    memesql::Table::Header header;
    header.columns["column1"].type = memesql::ColumnFields::DataTypes::INT;
    header.columns["column1"].index = 0;
    header.columns["column2"].type = memesql::ColumnFields::DataTypes::STRING;
    header.columns["column2"].index = 1;

    memesql::Table table(header);
    table.create_record({{"column1", memesql::Cell(1)}, {"column2", memesql::Cell("hello")}});

    ASSERT_THROW(table.get_record(1), std::out_of_range);
}

TEST(TableTest, CreateRecord) {
    memesql::Table::Header header;
    header.columns["column1"].type = memesql::ColumnFields::DataTypes::INT;
    header.columns["column1"].index = 0;
    header.columns["column2"].type = memesql::ColumnFields::DataTypes::STRING;
    header.columns["column2"].index = 1;

    memesql::Table table(header);
    table.create_record({{"column1", memesql::Cell(1)}, {"column2", memesql::Cell("hello")}});

    const auto& records = table.get_all_records();
    ASSERT_EQ(records.size(), 1);
}

TEST(TableTest, DeleteRecord) {
    memesql::Table::Header header;
    header.columns["column1"].type = memesql::ColumnFields::DataTypes::INT;
    header.columns["column1"].index = 0;
    header.columns["column2"].type = memesql::ColumnFields::DataTypes::STRING;
    header.columns["column2"].index = 1;

    memesql::Table table(header);
    table.create_record({{"column1", memesql::Cell(1)}, {"column2", memesql::Cell("hello")}});
    table.create_record({{"column1", memesql::Cell(2)}, {"column2", memesql::Cell("world")}});

    table.delete_record(0);
    const auto& records = table.get_all_records();
    ASSERT_EQ(records.size(), 1);
}