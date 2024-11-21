#pragma once

#include "Table.hpp"
#include <map>
#include <memory>
#include <string>

namespace memesql {
class Response;

class DataBase {
  public:
    Response execute(const std::string& query);

    void load_from_file(const std::string& path);
    void save_to_file(const std::string& path);

  private:
    std::map<std::string, std::shared_ptr<Table>> m_tables;

    void create_table(const std::string& table_name, const Table::Header& header);
    void drop_table(const std::string& table_name);
};
} // namespace memesql