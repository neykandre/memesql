#pragma once

#include "Cell.hpp"
#include "Definitions.hpp"
#include <map>
#include <memory>
#include <optional>
#include <vector>

namespace memesql {

class Record;

class Table {
  public:
    using RecordList = std::vector<std::shared_ptr<Record>>;
    struct Header {
        struct Column {
            ColumnFields::DataTypes type;
            ColumnFields::Attributes attributes;
            size_t index;

            std::optional<Cell> default_value;
            bool has_default_value = false;

            std::optional<int> length;

            bool order_indexed   = false;
            bool unorder_indexed = false;
        };
        std::map<std::string, Column> columns;
    };

    explicit Table(const Header& header);

    const Header& get_header() const;

    const RecordList& get_all_records() const;
    std::shared_ptr<Record> get_record(size_t index) const;

    void create_record(std::map<std::string, Cell> cells);
    void delete_record(std::shared_ptr<Record> record);

  private:
    Header m_header;
    RecordList m_records;
};

} // namespace memesql