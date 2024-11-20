#pragma once

#include "result.hpp"

#include <string_view>

namespace parser {
    template<typename P, typename F>
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

    template<typename P, typename F>
    ParseCast<P, F> cast(P parser, F func) {
        return ParseCast<P, F>(std::move(parser), std::move(func));
    }
}
