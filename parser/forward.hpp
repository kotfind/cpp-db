#pragma once

#include "result.hpp"
#include "concepts.hpp"

#include <string_view>
#include <tuple>

namespace parser {
    template<is_parser P>
    class ParseForward {
        public:
            using type = std::tuple<>;
            using result = ParseResult<type>;

            ParseForward(P parser)
              : parser(std::move(parser))
            {}

            result parse(std::string_view s) const {
                auto res = parser.parse(s);
                if (res.is_ok()) {
                    return result::ok({}, s);
                } else {
                    return result::fail();
                }
            }

        private:
            P parser;
    };

    template<is_parser P>
    ParseForward<P> fwd(P parser) {
        return ParseForward<P>(std::move(parser));
    }
}
