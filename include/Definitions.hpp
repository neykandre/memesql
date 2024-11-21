#pragma once

#include <compare>
#include <cstdint>
#include <ostream>
#include <string>
#include <string_view>
#include <variant>

namespace memesql {

using Null   = std::monostate;
using Int    = int32_t;
using Bool   = bool;
using String = std::string;

std::ostream& operator<<(std::ostream& os, const Null&);

class Bytes {
    std::string m_bytes;

  public:
    explicit Bytes(std::string_view sv);

    friend std::ostream& operator<<(std::ostream& os, const Bytes& bytes);

    std::strong_ordering operator<=>(const Bytes&) const = default;
};

namespace ColumnFields {
enum class Attributes {
    KEY           = 1L << 0,
    NOT_NULL      = 1L << 1,
    UNIQUE        = 1L << 2,
    AUTOINCREMENT = 1L << 3
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

enum class ArithmeticOperator {
    ADD,
    SUB,
    MUL,
    DIV,
    MOD
};

enum class ComparisonOperator {
    EQUAL,
    NOT_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL
};

enum class Logical {
    AND,
    OR,
    XOR,
    NOT
};

enum class QueryType {
    SELECT,
    INSERT,
    UPDATE,
    DELETE,
    DROP
};

enum class JoinType {
    INNER,
    LEFT,
    RIGHT,
    FULL
};

} // namespace memesql