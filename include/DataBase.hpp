#pragma once

// #include "Command/CreateCommand.hpp"
// #include "Command/DeleteCommand.hpp"
// #include "Command/InsertCommand.hpp"
// #include "Command/SelectCommand.hpp"
// #include "Command/UpdateCommand.hpp"
#include "Response.hpp"
#include "Table.hpp"
#include <map>
#include <memory>
#include <string>

namespace memesql {
namespace internal {
class Command;
class SelectCommand;
class CreateCommand;
class InsertCommand;
class DeleteCommand;
class UpdateCommand;
} // namespace internal

class DataBase {
    friend class internal::SelectCommand;
    friend class internal::CreateCommand;
    friend class internal::InsertCommand;
    friend class internal::DeleteCommand;
    friend class internal::UpdateCommand;


  public:
    using TableList = std::map<std::string, std::shared_ptr<internal::Table>>;
    Response execute(const std::string& query);

    void load_from_file(const std::string& path);
    void save_to_file(const std::string& path);

  private:
    TableList m_tables;
};
} // namespace memesql