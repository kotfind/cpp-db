#include "Value.hpp"

#include <cassert>
#include <cstdint>
#include <ios>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <variant>

std::ostream& operator<<(std::ostream& out, ValueType v) {
    switch (v) {
        case ValueType::INT:
            out << "int";
            break;

        case ValueType::BOOL:
            out << "bool";
            break;

        case ValueType::STRING:
            out << "string";
            break;

        case ValueType::BYTES:
            out << "bytes";
            break;

        default:
            // Unreachable
            assert(false);
    }
    return out;
}

Value Value::from_int(int32_t value) {
    return Value(std::move(value));
}


Value Value::from_bool(bool value) {
    return Value(std::move(value));
}

Value Value::from_string(std::string value) {
    return Value(std::move(value));
}

Value Value::from_bytes(std::vector<std::byte> value) {
    return Value(std::move(value));
}

Value::Value(int32_t value)
  : value(std::move(value))
{}

Value::Value(bool value)
  : value(std::move(value))
{}

Value::Value(std::string value)
  : value(std::move(value))
{}

Value::Value(std::vector<std::byte> value)
  : value(std::move(value))
{}

ValueType Value::get_type() const {
    if (std::holds_alternative<int32_t>(value)) {
        return ValueType::INT;
    } 
    else if (std::holds_alternative<bool>(value)) {
        return ValueType::BOOL;
    } 
    else if (std::holds_alternative<std::string>(value)) {
        return ValueType::STRING;
    } 
    else if (std::holds_alternative<std::vector<std::byte>>(value)) {
        return ValueType::BYTES;
    }  else {
        // Unreachable
        assert(false);
    }
}

bool Value::is_int() const {
    return std::holds_alternative<int32_t>(value);
}
bool Value::is_bool() const {
    return std::holds_alternative<bool>(value);
}
bool Value::is_string() const {
    return std::holds_alternative<std::string>(value);
}
bool Value::is_bytes() const {
    return std::holds_alternative<std::vector<std::byte>>(value);
}

const int32_t& Value::get_int() const {
    if (!std::holds_alternative<int32_t>(value)) {
        std::stringstream ss;
        ss << "get_int failed, value has type " << get_type();
        throw std::runtime_error(ss.str());
    }
    return std::get<int32_t>(value);
}
const bool& Value::get_bool() const {
    if (!std::holds_alternative<bool>(value)) {
        std::stringstream ss;
        ss << "get_bool failed, value has type " << get_type();
        throw std::runtime_error(ss.str());
    }
    return std::get<bool>(value);
}
const std::string& Value::get_string() const {
    if (!std::holds_alternative<std::string>(value)) {
        std::stringstream ss;
        ss << "get_string failed, value has type " << get_type();
        throw std::runtime_error(ss.str());
    }
    return std::get<std::string>(value);
}
const std::vector<std::byte>& Value::get_bytes() const {
    if (!std::holds_alternative<std::vector<std::byte>>(value)) {
        std::stringstream ss;
        ss << "get_bytes failed, value has type " << get_type();
        throw std::runtime_error(ss.str());
    }
    return std::get<std::vector<std::byte>>(value);
}

int32_t& Value::get_int() {
    if (!std::holds_alternative<int32_t>(value)) {
        std::stringstream ss;
        ss << "get_int failed, value has type " << get_type();
        throw std::runtime_error(ss.str());
    }
    return std::get<int32_t>(value);
}
bool& Value::get_bool() {
    if (!std::holds_alternative<bool>(value)) {
        std::stringstream ss;
        ss << "get_bool failed, value has type " << get_type();
        throw std::runtime_error(ss.str());
    }
    return std::get<bool>(value);
}
std::string& Value::get_string() {
    if (!std::holds_alternative<std::string>(value)) {
        std::stringstream ss;
        ss << "get_string failed, value has type " << get_type();
        throw std::runtime_error(ss.str());
    }
    return std::get<std::string>(value);
}
std::vector<std::byte>& Value::get_bytes() {
    if (!std::holds_alternative<std::vector<std::byte>>(value)) {
        std::stringstream ss;
        ss << "get_bytes failed, value has type " << get_type();
        throw std::runtime_error(ss.str());
    }
    return std::get<std::vector<std::byte>>(value);
}

const int32_t* Value::try_get_int() const { return std::get_if<int32_t>(&value); }
const bool* Value::try_get_bool() const { return std::get_if<bool>(&value); }
const std::string* Value::try_get_string() const  { return std::get_if<std::string>(&value); }
const std::vector<std::byte>* Value::try_get_bytes() const { return std::get_if<std::vector<std::byte>>(&value); }

int32_t* Value::try_get_int() { return std::get_if<int32_t>(&value); }
bool* Value::try_get_bool() { return std::get_if<bool>(&value); }
std::string* Value::try_get_string() { return std::get_if<std::string>(&value); }
std::vector<std::byte>* Value::try_get_bytes() { return std::get_if<std::vector<std::byte>>(&value); }

Value operator+(const Value& lhs, const Value& rhs) {
    if (lhs.is_string()) {
        return Value::from_string(lhs.get_string() + rhs.get_string());
    } else {
        return Value::from_int(lhs.get_int() + rhs.get_int());
    }
}
Value operator-(const Value& lhs, const Value& rhs) {
    return Value::from_int(lhs.get_int() - rhs.get_int());
}
Value operator*(const Value& lhs, const Value& rhs) {
    return Value::from_int(lhs.get_int() * rhs.get_int());
}
Value operator/(const Value& lhs, const Value& rhs) {
    return Value::from_int(lhs.get_int() / rhs.get_int());
}
Value operator%(const Value& lhs, const Value& rhs) {
    return Value::from_int(lhs.get_int() % rhs.get_int());
}
Value operator-(const Value& arg) {
    return Value::from_int(-arg.get_int());
}

bool operator<(const Value& lhs, const Value& rhs) {
    if (lhs.get_type() != rhs.get_type()) {
        throw std::runtime_error("cannot compare values of different types");
    }
    return lhs.value < rhs.value;
}
bool operator>(const Value& lhs, const Value& rhs) {
    if (lhs.get_type() != rhs.get_type()) {
        throw std::runtime_error("cannot compare values of different types");
    }
    return lhs.value > rhs.value;
}
bool operator<=(const Value& lhs, const Value& rhs) {
    if (lhs.get_type() != rhs.get_type()) {
        throw std::runtime_error("cannot compare values of different types");
    }
    return lhs.value <= rhs.value;
}
bool operator>=(const Value& lhs, const Value& rhs) {
    if (lhs.get_type() != rhs.get_type()) {
        throw std::runtime_error("cannot compare values of different types");
    }
    return lhs.value >= rhs.value;
}
bool operator==(const Value& lhs, const Value& rhs) {
    if (lhs.get_type() != rhs.get_type()) {
        throw std::runtime_error("cannot compare values of different types");
    }
    return lhs.value == rhs.value;
}
bool operator!=(const Value& lhs, const Value& rhs) {
    if (lhs.get_type() != rhs.get_type()) {
        throw std::runtime_error("cannot compare values of different types");
    }
    return lhs.value != rhs.value;
}

bool operator||(const Value& lhs, const Value& rhs) {
    return lhs.get_bool() || rhs.get_bool();
}
bool operator&&(const Value& lhs, const Value& rhs) {
    return lhs.get_bool() && rhs.get_bool();
}
bool operator^(const Value& lhs, const Value& rhs) {
    return lhs.get_bool() ^ rhs.get_bool();
}
bool operator!(const Value& arg) {
    return !arg.get_bool();
}

std::ostream& operator<<(std::ostream& out, const Value& value) {
    if (auto* v = value.try_get_bool()) {
        if (*v) {
            out << "true";
        } else {
            out << "false";
        }
    } else if (auto* v = value.try_get_int()) {
        out << *v;
    } else if (auto* v = value.try_get_string()) {
        out << '"';
        for (char ch : *v) {
            switch (ch) {
                case '\n':
                    out << "\\n";
                    break;

                case '\t':
                    out << "\\t";
                    break;

                case '\\':
                    out << "\\\\";
                    break;

                case '\r':
                    out << "\\r";
                    break;

                default:
                    out << ch;
                    break;
            }
        }
        out << '"';
    } else if (auto* v = value.try_get_bytes()) {
        out << "0x";

        auto it = v->begin();
        while (it != v->end()) {
            auto h = (int)(*it) / 16;
            auto l = (int)(*it) % 16;
            ++it;
            out << std::hex << h << l;
        }
    } else {
        assert(false);
    }
    return out;
}
