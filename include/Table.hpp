#pragma once

#include "Cell.hpp"
#include "Definitions.hpp"
#include "Record.hpp"
#include <unordered_map>
#include <memory>
#include <optional>
#include <vector>

namespace memesql {

class Table {
  public:
    using RecordList = std::vector<std::shared_ptr<Record>>;
    struct Header {
        struct Column {
            ColumnFields::DataTypes type;
            ColumnFields::Attributes attributes;
            size_t index;

            std::optional<Cell> default_value;

            std::optional<int> length;

            bool order_indexed   = false;
            bool unorder_indexed = false;
        };
        std::unordered_map<std::string, Column> columns;
    };

    explicit Table(const Header& header);

    const Header& get_header() const;

    const RecordList& get_all_records() const;
    std::shared_ptr<Record> get_record(size_t index) const;

    void create_record(std::unordered_map<std::string, Cell> cells);
    void delete_record(size_t index);

  private:
    Header m_header;
    RecordList m_records;
};

} // namespace memesql