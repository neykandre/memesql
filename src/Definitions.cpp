#include "Definitions.hpp"
#include <iomanip>

namespace memesql {

namespace ColumnFields {
bool Attributes::is_autoincrement() const {
    return autoincrement;
}
bool Attributes::is_key() const {
    return key;
}
bool Attributes::is_not_null() const {
    return not_null;
}
bool Attributes::is_unique() const {
    return unique;
}
void Attributes::add_autoincrement() {
    autoincrement = true;
}
void Attributes::add_key() {
    key = true;
}
void Attributes::add_not_null() {
    not_null = true;
}
void Attributes::add_unique() {
    unique = true;
}
} // namespace ColumnFields

Bytes::Bytes(std::string_view sv) {
    if (sv.substr(0, 2) == "0x") {
        if (sv.size() % 2 != 0) {
            throw std::invalid_argument("Invalid bytes format");
        }

        for (size_t i = 2; i < sv.size(); i += 2) {
            m_bytes.push_back(
                std::stoul(std::string{ sv.substr(i, 2) }, nullptr, 16));
        }
    } else {
        m_bytes = sv;
    }
}

size_t Bytes::size() const {
    return m_bytes.size();
}

std::ostream& operator<<(std::ostream& os, const Bytes& bytes) {
    if (os.flags() & std::ios_base::hex) {
        os << "0x";
        for (const auto& byte : bytes.m_bytes) {
            os << std::hex << std::setw(2) << std::setfill('0')
               << static_cast<int>(byte);
        }
    } else {
        os << bytes.m_bytes;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Null&) {
    return os << "<null>";
}
} // namespace memesql