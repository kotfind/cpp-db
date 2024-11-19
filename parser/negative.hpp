#pragma once

#include "result.hpp"

#include <string_view>
#include <variant>

template<typename P>
class ParseNegative {
    public:
        using type = std::monostate;
        using result = ParseResult<type>;

        ParseNegative(P parser)
          : parser(std::move(parser))
        {}

        result parse(std::string_view s) {
            auto res = parser.parse(s);
            if (res.is_ok()) {
                return result::fail();
            } else {
                return result::ok(std::monostate {}, s);
            }
        }

    private:
        P parser;
};

template<typename P>
ParseNegative<P> neg(P parser) {
    return ParseNegative<P>(std::move(parser));
}
