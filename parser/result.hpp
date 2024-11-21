#pragma once

#include <string_view>
#include <string>
#include <cassert>
#include <variant>
#include <vector>

namespace parser {
    using ExpectedSetType = std::vector<std::string>;

    template<typename T>
    class ParseResult {
        private:
            struct ParseFail {
                ExpectedSetType expected_set;
            };

            struct ParseOk {
                T value;
            };

        public:
            static ParseResult ok(T t, std::string_view s) {
                return ParseResult(ParseOk{std::move(t)}, s);
            }

            static ParseResult fail(ExpectedSetType expected_set, std::string_view s) {
                return ParseResult(ParseFail{std::move(expected_set)}, s);
            }

            template<typename U>
            static ParseResult fail(ParseResult<U> other) {
                return ParseResult(ParseFail{std::move(other.expected_set())}, other.str());
            }

            const T& value() const {
                assert(is_ok());
                return std::get<ParseOk>(res).value;
            }

            const ExpectedSetType& expected_set() const {
                assert(is_fail());
                return std::get<ParseFail>(res).expected_set;
            }

            std::string_view str() const {
                return s;
            }

            bool is_ok() const {
                return std::holds_alternative<ParseOk>(res);
            }

            bool is_fail() const {
                return !is_ok();
            }

        private:
            ParseResult(std::variant<ParseOk, ParseFail> res, std::string_view s)
              : s(s),
                res(std::move(res))
            {}

            std::string_view s;
            std::variant<ParseOk, ParseFail> res;
    };
}
