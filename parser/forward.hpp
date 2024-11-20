#pragma once

#include "result.hpp"

#include <string_view>
#include <tuple>

namespace parser {
    template<typename P>
    class ParseForward {
        public:
            using type = std::tuple<>;
            using result = ParseResult<type>;

            ParseForward(P parser)
              : parser(std::move(parser))
            {}

            result parse(std::string_view s) {
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

    template<typename P>
    ParseForward<P> fwd(P parser) {
        return ParseForward<P>(std::move(parser));
    }
}
