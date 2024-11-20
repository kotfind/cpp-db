#pragma once

#include "result.hpp"

#include <string_view>
#include <tuple>

namespace parser {
    namespace __impl {
        template<typename... Ts>
        struct join;

        template<typename T, typename... Ts>
        struct join<T, std::tuple<Ts...>> {
            using type = std::tuple<T, Ts...>;
            type value;

            join(T t, std::tuple<Ts...> ts)
              : value(std::tuple_cat(
                    std::tuple(std::move(t)),
                    std::move(ts)
                ))
            {}
        };

        template<typename... Ts>
        struct join<std::tuple<>, std::tuple<Ts...>> {
            using type = std::tuple<Ts...>;
            type value;

            join(std::tuple<>, std::tuple<Ts...> ts)
               : value(std::move(ts))
            {}
        };

        template<>
        struct join<std::tuple<>, std::tuple<>> {
            using type = std::tuple<>;
            type value;

            join(std::tuple<>, std::tuple<>)
               : value(std::tuple())
            {}
        };
    }

    template<typename... Ps>
    class ParseSeq;

    template<typename P, typename... Ps>
    class ParseSeq<P, Ps...> {
        private:
            using join_ = __impl::join<typename P::type, typename ParseSeq<Ps...>::type>;

        public:
            using type = join_::type;
            using result = ParseResult<type>;

            ParseSeq(P parser1, Ps... parsers)
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
                    std::move(join_(
                        std::move(res1.value()),
                        std::move(res2.value())
                    ).value),
                    res2.str()
                );
            }

        private:
            P parser1;
            ParseSeq<Ps...> parser2;
    };

    template<>
    class ParseSeq<> {
        public:
            using type = std::tuple<>;
            using result = ParseResult<type>;

            result parse(std::string_view s) {
                return result::ok({}, s);
            }
    };

    template<typename... Ps>
    ParseSeq<Ps...> seq(Ps... ps) {
        return ParseSeq<Ps...>(std::move(ps)...);
    }
}
