#include "parser.hpp"

#include <charconv>
#include <optional>
#include <system_error>

namespace parse_atoms {
    using namespace parser;

    inline auto num() {
        return try_cast(
            view(seq(
                opt(any(c('+'), c('-'))),
                c('1', '9'),
                rep(c('0', '9'))
            )),
            [](auto str) -> std::optional<int> {
                int ans;

                // from_chars refuses to parse leading +
                if (str.starts_with('+')) {
                    str.remove_prefix(1);
                }

                auto res = std::from_chars(str.begin(), str.end(), ans);
                if (res.ec == std::errc {}) {
                    return {ans};
                } else {
                    return std::nullopt;
                }
            }
        );
    }

    static auto ws_char() {
        return any(
            c(' '),
            c('\n'),
            c('\t'),
            c('\r')
        );
    }

    static auto comment() {
        return any(
            // /* comment */
            ignore(seq(
                s("/*"),
                rep(seq(
                    neg(s("*/")),
                    c()
                )),
                s("*/")
            )),
            // // comment
            ignore(seq(
                s("//"),
                rep(seq(
                    neg(s("\n")),
                    c()
                )),
                s("\n")
            ))
        );
    }

    inline auto ws() {
        return ignore(
            rep(any(
                ignore(ws_char()),
                ignore(comment())
            ))
        );
    }

    inline auto ws1() {
        return ignore(
            rep1(any(
                ignore(ws_char()),
                ignore(comment())
            ))
        );
    }
}
