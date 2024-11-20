#pragma once

#include "result.hpp"

#include <string_view>
#include <tuple>

namespace parser {
    template<typename P>
    class ParseNegative {
        public:
            using type = std::tuple<>;
            using result = ParseResult<type>;

            ParseNegative(P parser)
              : parser(std::move(parser))
            {}

            result parse(std::string_view s) const {
                auto res = parser.parse(s);
                if (res.is_ok()) {
                    return result::fail();
                } else {
                    return result::ok({}, s);
                }
            }

        private:
            P parser;
    };

    template<typename P>
    ParseNegative<P> neg(P parser) {
        return ParseNegative<P>(std::move(parser));
    }
}
