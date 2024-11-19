#pragma once

#include "result.hpp"

#include <string_view>

template<typename P>
class ParseOption {
    public:
        using type = std::optional<typename P::type>;
        using result = ParseResult<type>;

        ParseOption(P parser)
          : parser(std::move(parser))
        {}

        result parse(std::string_view s) {
            auto res = parser.parse(s);
            if (res.is_ok()) {
                return result::ok({res.value()}, res.str());
            } else {
                return result::ok(std::nullopt, s);
            }
        }

    private:
        P parser;
};

template<typename P>
ParseOption<P> opt(P parser) {
    return ParseOption<P>(std::move(parser));
}