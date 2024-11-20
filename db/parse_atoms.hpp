#include "parser.hpp"

#include <charconv>
#include <cstddef>
#include <optional>
#include <system_error>

namespace parse_atoms {
    using namespace parser;

    inline auto ws_char() {
        return any(
            c(' '),
            c('\n'),
            c('\t'),
            c('\r')
        );
    }

    inline auto comment() {
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
        return ignore(rep(any(
            ignore(ws_char()),
            ignore(comment())
        )));
    }

    inline auto ws1() {
        return ignore(rep1(any(
            ignore(ws_char()),
            ignore(comment())
        )));
    }

    inline auto string_char() {
        return any(
            cast(s("\\n"), [](auto) { return '\n'; }),
            cast(s("\\r"), [](auto) { return '\r'; }),
            cast(s("\\t"), [](auto) { return '\t'; }),
            cast(s("\\\\"), [](auto) { return '\\'; }),
            cast(s("\\\""), [](auto) { return '\"'; }),
            seq(neg(c('"')), c())
        );
    }

    inline auto string() {
        return cast(
            seq(
                ignore(c('"')),
                rep(string_char()), // str
                ignore(c('"'))
            ),
            [](auto str) {
                return std::string(str.begin(), str.end());
            }
        );
    }

    inline auto boolean() {
        return any(
            cast(S("true"), [](auto) { return true; }),
            cast(S("false"), [](auto) { return false; })
        );
    }

    inline auto bytes() {
        return cast(
            seq(
                ignore(s("0x")),
                rep1(any(
                    cast(c('0', '9'), [](char ch) { return (int)(ch - '0'); }),
                    cast(c('a', 'z'), [](char ch) { return (int)(ch - 'a' + 10); })
                ))
            ),
            [](auto digits16) {
                std::vector<std::byte> ans;
                ans.reserve((digits16.size() + 1) / 2);
                auto it = digits16.begin();
                if (digits16.size() % 2 == 1) {
                    ans.push_back(std::byte(*it));
                    ++it;
                }
                while (it != digits16.end()) {
                    auto h = *it;
                    ++it;
                    auto l = *it;
                    ++it;
                    ans.push_back(std::byte(h * 16 + l));
                }
                return ans;
            }
        );
    }

    inline auto num() {
        return try_cast(
            view(seq(
                neg(s("0x")),
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
}
