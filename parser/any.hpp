#pragma once

#include "result.hpp"

#include <string_view>
#include <type_traits>

namespace parser {
    template<typename P, typename... Ps>
    class ParseAny {
        static_assert(
            std::conjunction_v<std::is_same<typename P::type, typename Ps::type>...>,
            "result types of all the parsers should be the same"
        );

        public:
            using type = typename P::type;
            using result = ParseResult<type>;

            ParseAny(P parser1, Ps... parsers)
              : parser1(std::move(parser1)),
                parser2(std::move(parsers)...)
            {}

            result parse(std::string_view s) const {
                auto res1 = parser1.parse(s);
                if (res1.is_ok()) {
                    return res1;
                }

                return parser2.parse(s);
            }

        private:
            P parser1;
            ParseAny<Ps...> parser2;
    };

    template<typename P>
    class ParseAny<P> {
        public:
            using type = typename P::type;
            using result = ParseResult<type>;

            ParseAny(P parser)
              : parser(std::move(parser))
            {}

            result parse(std::string_view s) const {
                return parser.parse(s);
            }

        private:
            P parser;
    };

    template<typename... Ps>
    ParseAny<Ps...> any(Ps... ps) {
        return ParseAny(std::move(ps)...);
    }
}
