#pragma once

#include <optional>
#include <string_view>
#include <cassert>

namespace parser {
    template<typename T>
    class ParseResult {
        public:
            static ParseResult ok(T t, std::string_view s) {
                return ParseResult({{std::move(t), s}});
            }

            static ParseResult fail() {
                return ParseResult(std::nullopt);
            }

            const T& value() const {
                assert(is_ok());
                return inner->first;
            }

            T& value() {
                assert(is_ok());
                return inner->first;
            }

            std::string_view str() const {
                assert(is_ok());
                return inner->second;
            }

            bool is_ok() const {
                return inner.has_value();
            }

            bool is_fail() const {
                return !is_ok();
            }

        private:
            ParseResult(std::optional<std::pair<T, std::string_view>> inner_) : inner(std::move(inner_)) {}

            std::optional<std::pair<T, std::string_view>> inner;
    };
}
