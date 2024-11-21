#pragma once

#include "Cell.hpp"

namespace memesql {
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
} // namespace memesql