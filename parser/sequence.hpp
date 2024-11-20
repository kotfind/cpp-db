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

        template<typename... Ps>
        class ParseSeqInner;

        template<typename P, typename... Ps>
        class ParseSeqInner<P, Ps...> {
            private:
                using join_ = __impl::join<typename P::type, typename ParseSeqInner<Ps...>::type>;

            public:
                using type = join_::type;
                using result = ParseResult<type>;

                ParseSeqInner(P parser1, Ps... parsers)
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
                ParseSeqInner<Ps...> parser2;
        };

        template<>
        class ParseSeqInner<> {
            public:
                using type = std::tuple<>;
                using result = ParseResult<type>;

                result parse(std::string_view s) {
                    return result::ok({}, s);
                }
        };

        template<typename T>
        struct unwrap_singleton;

        template<typename... Ts>
        struct unwrap_singleton<std::tuple<Ts...>> {
            using type = std::tuple<Ts...>;
            type value;

            unwrap_singleton(std::tuple<Ts...> ts)
              : value(std::move(ts))
            {}
        };

        template<typename T>
        struct unwrap_singleton<std::tuple<T>> {
            using type = T;
            type value;

            unwrap_singleton(std::tuple<T> t)
              : value(std::move(std::get<0>(t)))
            {}
        };
    }

    template<typename... Ps>
    class ParseSeq {
        private:
            using unwrap_ = __impl::unwrap_singleton<typename __impl::ParseSeqInner<Ps...>::type>;

        public:
            using type = unwrap_::type;
            using result = ParseResult<type>;

            ParseSeq(Ps... ps)
              : parser({std::move(ps)...})
            {}

            result parse(std::string_view s) {
                auto res = parser.parse(s);
                if (res.is_ok()) {
                    return result::ok(
                        std::move(unwrap_(std::move(res.value())).value),
                        res.str()
                    );
                } else {
                    return result::fail();
                }
            }

        private:
            __impl::ParseSeqInner<Ps...> parser;
    };

    template<typename... Ps>
    ParseSeq<Ps...> seq(Ps... ps) {
        return ParseSeq<Ps...>(std::move(ps)...);
    }
}
