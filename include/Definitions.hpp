#pragma once

#include "Exceptions.hpp"
#include <compare>
#include <cstdint>
#include <ostream>
#include <string>
#include <string_view>
#include <sys/types.h>
#include <variant>
#include <vector>

namespace memesql {

using Null   = std::monostate;
using Int    = int32_t;
using Bool   = bool;
using String = std::string;

std::ostream& operator<<(std::ostream& os, const Null&);

class Bytes {
    std::vector<uint8_t> m_bytes;

  public:
    explicit Bytes(std::string_view sv);

    friend std::ostream& operator<<(std::ostream& os, const Bytes& bytes);

    std::strong_ordering operator<=>(const Bytes&) const = default;

    size_t size() const;
};

namespace internal {

namespace ColumnFields {

class Attributes {
  public:
    Attributes() = default;

    bool is_autoincrement() const;
    bool is_key() const;
    bool is_not_null() const;
    bool is_unique() const;

    void add_autoincrement();
    void add_key();
    void add_not_null();
    void add_unique();

  private:
    bool autoincrement = false;
    bool unique        = false;
    bool not_null      = false;
    bool key           = false;
};

enum class DataTypes {
    INT,
    BOOL,
    STRING,
    BYTES
};

template <DataTypes T>
struct DataTypeTraits {};

template <>
struct DataTypeTraits<DataTypes::INT> {
    using type = Int;
};

template <>
struct DataTypeTraits<DataTypes::BOOL> {
    using type = Bool;
};

template <>
struct DataTypeTraits<DataTypes::STRING> {
    using type = String;
};

template <>
struct DataTypeTraits<DataTypes::BYTES> {
    using type = Bytes;
};

} // namespace ColumnFields

enum class BinaryOpType {
    ARITHM_START,
    PLUS,
    MINUS,
    MUL,
    DIV,
    MOD,
    ARITHM_END,

    COMPARE_START,
    EQUAL,
    NOT_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,
    COMPARE_END,

    LOGIC_START,
    AND,
    OR,
    XOR,
    LOGIC_END
};

enum class UnaryOpType {
    NOT,
    MINUS,
    LENGTH
};
} // namespace internal
} // namespace memesql