#pragma once

#include "parser.hpp"

#include <cassert>
#include <charconv>
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

    friend bool operator<(const Value& lhs, const Value& rhs);
    friend bool operator>(const Value& lhs, const Value& rhs);
    friend bool operator<=(const Value& lhs, const Value& rhs);
    friend bool operator>=(const Value& lhs, const Value& rhs);
    friend bool operator==(const Value& lhs, const Value& rhs);
    friend bool operator!=(const Value& lhs, const Value& rhs);
};

Value operator+(const Value& lhs, const Value& rhs);
Value operator-(const Value& lhs, const Value& rhs);
Value operator*(const Value& lhs, const Value& rhs);
Value operator/(const Value& lhs, const Value& rhs);
Value operator%(const Value& lhs, const Value& rhs);
Value operator-(const Value& arg);

bool operator||(const Value& lhs, const Value& rhs);
bool operator&&(const Value& lhs, const Value& rhs);
bool operator^(const Value& lhs, const Value& rhs);
bool operator!(const Value& arg);

namespace parse_atoms {
    using namespace parser;

    inline auto string_char() {
        return any(
            cast(s("\\n"), [](auto) { return '\n'; }),
            cast(s("\\r"), [](auto) { return '\r'; }),
            cast(s("\\t"), [](auto) { return '\t'; }),
            cast(s("\\\\"), [](auto) { return '\\'; }),
            cast(s("\\\""), [](auto) { return '\"'; }),
            seq(neg(c('"')), c())
        );
    }

    inline auto string() {
        return cast(
            seq(
                ignore(c('"')),
                rep(string_char()), // str
                ignore(c('"'))
            ),
            [](auto str) {
                return std::string(str.begin(), str.end());
            }
        );
    }

    inline auto boolean() {
        return any(
            cast(S("true"), [](auto) { return true; }),
            cast(S("false"), [](auto) { return false; })
        );
    }

    inline auto bytes() {
        return cast(
            seq(
                ignore(s("0x")),
                rep1(any(
                    cast(c('0', '9'), [](char ch) { return (int)(ch - '0'); }),
                    cast(c('a', 'z'), [](char ch) { return (int)(ch - 'a' + 10); })
                ))
            ),
            [](auto digits16) {
                std::vector<std::byte> ans;
                ans.reserve((digits16.size() + 1) / 2);
                auto it = digits16.begin();
                if (digits16.size() % 2 == 1) {
                    ans.push_back(std::byte(*it));
                    ++it;
                }
                while (it != digits16.end()) {
                    auto h = *it;
                    ++it;
                    auto l = *it;
                    ++it;
                    ans.push_back(std::byte(h * 16 + l));
                }
                return ans;
            }
        );
    }

    inline auto num() {
        return try_cast(
            view(seq(
                neg(s("0x")),
                opt(any(c('+'), c('-'))),
                c('1', '9'),
                rep(c('0', '9'))
            )),
            [](auto str) -> std::optional<int> {
                int ans;

                // from_chars refuses to parse leading +
                if (str.starts_with('+')) {
                    str.remove_prefix(1);
                }

                auto res = std::from_chars(str.begin(), str.end(), ans);
                if (res.ec == std::errc {}) {
                    return {ans};
                } else {
                    return std::nullopt;
                }
            }
        );
    }

    inline auto value() {
        return any(
            cast(string(),  [](auto v) { return Value::from_string(v); }),
            cast(bytes(),   [](auto v) { return Value::from_bytes(v); }),
            cast(boolean(), [](auto v) { return Value::from_bool(v); }),
            cast(num(),     [](auto v) { return Value::from_int(v); })
        );
    }
}
