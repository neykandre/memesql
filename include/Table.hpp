#pragma once

#include "Cell.hpp"
#include "Definitions.hpp"
#include "Record.hpp"
#include <unordered_map>
#include <memory>
#include <optional>
#include <list>

namespace memesql {

class Table {
  public:
    using RecordList = std::list<std::shared_ptr<Record>>;
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

    RecordList::const_iterator create_record(std::unordered_map<std::string, Cell> cells);
    RecordList::const_iterator delete_record(RecordList::const_iterator);
    RecordList::const_iterator update_record(RecordList::const_iterator, std::unordered_map<std::string, Cell> update_cells);

  private:
    Header m_header;
    RecordList m_records;
};

} // namespace memesql