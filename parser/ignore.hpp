#pragma once

#include "result.hpp"
#include "concepts.hpp"

#include <string_view>
#include <tuple>

namespace parser {
    template<is_parser P>
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
                    return result::fail(std::move(res));
                }
            }

        private:
            P parser;
    };

    template<is_parser P>
    ParseIgnore<P> ignore(P parser) {
        return ParseIgnore<P>(std::move(parser));
    }
}
