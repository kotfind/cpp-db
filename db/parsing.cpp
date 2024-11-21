#include "parsing.hpp"

#include "Value.hpp"

#include <charconv>
#include <parser.hpp>
#include <string>
#include <tuple>

using namespace parser;

// -------------------- Whitespace --------------------
static is_parser_for<char> auto ws_char =
    any(
        c(' '),
        c('\n'),
        c('\t'),
        c('\r')
    );

static is_parser_for<std::tuple<>> auto comment =
    any(
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

static is_parser_for<std::tuple<>> auto ws =
    ignore(rep(any(
        ignore(ws_char),
        comment
    )));

static is_parser_for<std::tuple<>> auto ws1 =
    ignore(rep1(any(
        ignore(ws_char),
        comment
    )));

// -------------------- Value --------------------

static is_parser_for<char> auto string_char =
    any(
        cast(s("\\n"), [](auto) { return '\n'; }),
        cast(s("\\r"), [](auto) { return '\r'; }),
        cast(s("\\t"), [](auto) { return '\t'; }),
        cast(s("\\\\"), [](auto) { return '\\'; }),
        cast(s("\\\""), [](auto) { return '\"'; }),
        seq(neg(c('"')), c())
    );

static is_parser_for<std::string> auto string =
    cast(
        seq(
            ignore(c('"')),
            rep(string_char), // str
            ignore(c('"'))
        ),
        [](auto str) {
            return std::string(str.begin(), str.end());
        }
    );


static is_parser_for<bool> auto boolean =
    any(
        cast(S("true"), [](auto) { return true; }),
        cast(S("false"), [](auto) { return false; })
    );

static is_parser_for<std::vector<std::byte>> auto bytes =
    cast(
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

static is_parser_for<int32_t> auto num =
    try_cast(
        view(seq(
            neg(s("0x")),
            opt(any(c('+'), c('-'))),
            c('1', '9'),
            rep(c('0', '9'))
        )),
        [](auto str) -> std::optional<int> {
            int32_t ans;

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

static is_parser_for<Value> auto value =
    any(
        cast(string,  [](auto v) { return Value::from_string(v); }),
        cast(bytes,   [](auto v) { return Value::from_bytes(v); }),
        cast(boolean, [](auto v) { return Value::from_bool(v); }),
        cast(num,     [](auto v) { return Value::from_int(v); })
    );

parser::Parser<Value> value_parser = value;
