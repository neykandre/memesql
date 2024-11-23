#pragma once

#include "Table.hpp"
#include <map>
#include <memory>
#include <string>

namespace memesql {
class Response;

class DataBase {
    friend class SelectCommand;
    friend class CreateCommand;

  public:
    Response execute(const std::string& query);

    void load_from_file(const std::string& path);
    void save_to_file(const std::string& path);

  private:
    std::map<std::string, std::shared_ptr<Table>> m_tables;
};
} // namespace memesql