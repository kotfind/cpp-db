#pragma once

#include "concepts.hpp"
#include "result.hpp"

#include <string_view>

namespace parser {
    template<is_parser P, typename F>
    requires requires(F func, P::type value) {
        func(value);
    }
    class ParseCast {
        public:
            using type = std::invoke_result_t<F, typename P::type>;
            using result = ParseResult<type>;

            ParseCast(P parser, F func)
              : parser(std::move(parser)),
                func(std::move(func))
            {}

            result parse(std::string_view s) const {
                auto res = parser.parse(s);
                if (res.is_fail()) {
                    return result::fail();
                }
                return result::ok(func(std::move(res.value())), res.str());
            }

        private:
            P parser;
            F func;
    };

    template<is_parser P, typename F>
    requires requires(F func, P::type value) {
        func(value);
    }
    ParseCast<P, F> cast(P parser, F func) {
        return ParseCast<P, F>(std::move(parser), std::move(func));
    }
}
