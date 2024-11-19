#pragma once

#include <string_view>
#include <tuple>

#include "result.hpp"

template<typename P1, typename... Ps>
class ParseSeq {
    public:
        using type = std::tuple<typename P1::type, typename Ps::type...>;
        using result = ParseResult<type>;

        ParseSeq(P1 parser1, Ps... parsers)
          : parser1(std::move(parser1)),
            parser2(std::move(parsers)...)
        {}

        result parse(std::string_view s) {
            auto res1 = parser1.parse(s);
            if (res1.is_fail()) {
                return result::fail();
            }

            auto res2 = parser2.parse(res1.str());
            if (res2.is_fail()) {
                return result::fail();
            }

            return result::ok(
                std::tuple_cat(
                    std::tuple(std::move(res1.value())),
                    std::move(res2.value())
                ),
                res2.str()
            );
        }

    private:
        P1 parser1;
        ParseSeq<Ps...> parser2;
};

template<typename P>
class ParseSeq<P> {
    public:
        using type = std::tuple<typename P::type>;
        using result = ParseResult<type>;

        ParseSeq(P parser)
          : parser(std::move(parser))
        {}

        result parse(std::string_view s) {
            auto res = parser.parse(s);
            if (res.is_fail()) {
                return result::fail();
            }
            return result::ok({std::move(res.value())}, res.str());
        }

    private:
        P parser;
};

template<typename... Ps>
ParseSeq<Ps...> seq(Ps... ps) {
    return ParseSeq<Ps...>(std::move(ps)...);
}
