#pragma once

#include "result.hpp"

#include <string_view>
#include <tuple>

namespace parser {
    template<typename P>
    class ParseIgnore {
        public:
            using type = std::tuple<>;
            using result = ParseResult<type>;

            ParseIgnore(P parser)
              : parser(std::move(parser))
            {}

            result parse(std::string_view s) const {
                auto res = parser.parse(s);
                if (res.is_ok()) {
                    return result::ok({}, res.str());
                } else {
                    return result::fail();
                }
            }

        private:
            P parser;
    };

    template<typename P>
    ParseIgnore<P> ignore(P parser) {
        return ParseIgnore<P>(std::move(parser));
    }
}
