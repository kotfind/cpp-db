#pragma once

#include "result.hpp"

#include <string_view>

template<typename P>
class ParseForward {
    public:
        using type = typename P::type;
        using result = ParseResult<type>;

        ParseForward(P parser)
          : parser(std::move(parser))
        {}

        result parse(std::string_view s) {
            auto res = parser.parse(s);
            if (res.is_ok()) {
                return result::ok(res.value(), s);
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