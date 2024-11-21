#pragma once

#include "ColumnFields.hpp"
#include <list>
#include <map>
#include <memory>

namespace memesql {

class Record;

class Table {
    friend class DataBase;
    friend class Response;

  private:
    struct Header {
        struct Column {
            DataTypes type;
            Attributes attributes;
            size_t index;
        };
        std::map<std::string, Column> m_columns;
    };

    explicit Table(const Header& header);

  private:
    Header m_header;
    std::list<std::shared_ptr<Record>> m_records;

    void create_record(std::map<std::string, Cell> cells);
    void delete_record(std::shared_ptr<Record> record);
};

} // namespace memesql