#pragma once

#include "Cell.hpp"
#include "Definitions.hpp"
#include <list>
#include <map>
#include <memory>
#include <optional>

namespace memesql {

class Record;

class Table {
  public:
    using RecordList = std::list<std::shared_ptr<Record>>;
    struct Header {
        struct Column {
            ColumnFields::DataTypes type;
            ColumnFields::Attributes attributes;
            size_t index;

            std::optional<Cell> default_value;
            bool has_default_value    = false;
            
            std::optional<int> length = 0;
            
            bool order_indexed   = false;
            bool unorder_indexed = false;
        };
        std::map<std::string, Column> columns;
    };

    explicit Table(const Header& header);

    Header get_header() const;

    const RecordList& get_records() const;
    void create_record(std::map<std::string, Cell> cells);
    void delete_record(std::shared_ptr<Record> record);

  private:
    Header m_header;
    RecordList m_records;
};

} // namespace memesql