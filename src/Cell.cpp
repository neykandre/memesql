#include "Cell.hpp"
#include <cstring>
#include <iomanip>

namespace memesql {

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
        m_bytes = std::vector<uint8_t>(sv.begin(), sv.end());
    }
}

std::ostream& operator<<(std::ostream& os, const Bytes& bytes) {
    if (os.flags() & std::ios_base::hex) {
        os << "0x";
        for (const auto& byte : bytes.m_bytes) {
            os << std::hex << std::setw(2) << std::setfill('0')
               << static_cast<int>(byte);
        }
    } else {
        for (const auto& byte : bytes.m_bytes) {
            os << static_cast<char>(byte);
        }
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Null&) {
    return os << "<null>";
}

std::ostream& operator<<(std::ostream& os, const Cell& cell) {
    std::visit(
        [&os](auto&& value) {
            os << value;
        },
        cell.m_value);
    return os;
}
bool Cell::is_null() const {
    return std::holds_alternative<Null>(m_value);
}

} // namespace memesql