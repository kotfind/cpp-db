#pragma once

#include "result.hpp"

#include <string_view>
#include <optional>

namespace parser {
    namespace __impl {
        template<typename T>
        struct is_std_optional : std::false_type {};

        template<typename T>
        struct is_std_optional<std::optional<T>> : std::true_type {};
    }

    template<typename P, typename F>
    class ParseTryCast {
        static_assert(
            __impl::is_std_optional<std::invoke_result_t<F, typename P::type>>::value,
            "the function should return std::optional"
        );

        public:
            using type = std::invoke_result_t<F, typename P::type>::value_type;
            using result = ParseResult<type>;

            ParseTryCast(P parser, F func)
              : parser(std::move(parser)),
                func(std::move(func))
            {}

            result parse(std::string_view s) const {
                auto res = parser.parse(s);
                if (res.is_fail()) {
                    return result::fail();
                }
                auto cast_res = func(std::move(res.value()));
                if (!cast_res.has_value()) {
                    return result::fail();
                }
                return result::ok(std::move(*cast_res), res.str());
            }

        private:
            P parser;
            F func;
    };

    template<typename P, typename F>
    ParseTryCast<P, F> try_cast(P parser, F func) {
        return ParseTryCast<P, F>(std::move(parser), std::move(func));
    }
}
