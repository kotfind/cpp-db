#pragma once

#include "result.hpp"
#include "concepts.hpp"

#include <string_view>
#include <type_traits>

namespace parser {
    template<is_parser P, is_parser... Ps>
    requires (std::is_same<typename P::type, typename Ps::type>::value && ...) // all types should be the same
    class ParseAny {
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

    template<is_parser P>
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

    template<is_parser... Ps>
    ParseAny<Ps...> any(Ps... ps) {
        return ParseAny(std::move(ps)...);
    }
}
