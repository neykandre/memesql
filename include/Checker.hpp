#pragma once

#include "Cell.hpp"
#include "DataBase.hpp"
#include "Definitions.hpp"
#include "Expression/Expression.hpp"
#include "Record.hpp"
#include "Table.hpp"
#include <memory>

namespace memesql::internal {

class Checker {
    using CellMap = std::unordered_map<std::string, Cell>;

  public:
    struct TableExistsDTO {
        const DataBase::TableList& tables;
        const std::string& table_name;
    };

    struct CheckTypesDTO {
        std::shared_ptr<Table> table;
        const CellMap& cells;
    };

    struct AutocompleteDTO {
        const std::unordered_map<std::string, Cell>& input;
        const Table::Header& header;
        const Table::RecordList& records;
    };

    struct DuplicateDTO {
        Table::RecordList::const_iterator records_begin;
        Table::RecordList::const_iterator records_end;
        size_t column_index;
        const ColumnFields::Attributes& attributes;
        const Cell& cell;
    };

  public:
    static void check_table_exists(TableExistsDTO);
    static void check_types(CheckTypesDTO);
    static void check_header(const Table::Header&);
    static void check_is_const(std::shared_ptr<Expression>);
    static std::vector<Cell> autocomplete_record_map(AutocompleteDTO);
    static void check_duplicate(DuplicateDTO);
};
} // namespace memesql::internal