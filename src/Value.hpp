#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

enum class ValueType {
    INT,
    BOOL,
    STRING,
    BYTES,
};

std::ostream& operator<<(std::ostream& out, ValueType v);

class Value {
    public:
        static Value from_int(int32_t);
        static Value from_bool(bool);
        static Value from_string(std::string);
        static Value from_bytes(std::vector<std::byte>);

        ValueType get_type() const;

        bool is_int() const;
        bool is_bool() const;
        bool is_string() const;
        bool is_bytes() const;

        const int32_t& get_int() const;
        const bool& get_bool() const;
        const std::string& get_string() const;
        const std::vector<std::byte>& get_bytes() const;

        int32_t& get_int();
        bool& get_bool();
        std::string& get_string();
        std::vector<std::byte>& get_bytes();

        const int32_t* try_get_int() const;
        const bool* try_get_bool() const;
        const std::string* try_get_string() const;
        const std::vector<std::byte>* try_get_bytes() const;

        int32_t* try_get_int();
        bool* try_get_bool();
        std::string* try_get_string();
        std::vector<std::byte>* try_get_bytes();

    private:
        explicit Value(int32_t);
        explicit Value(bool);
        explicit Value(std::string);
        explicit Value(std::vector<std::byte>);

        std::variant<int32_t, bool, std::string, std::vector<std::byte>> value;

    friend bool operator==(const Value& lhs, const Value& rhs);
};
