#include "parsing.hpp"

#include "Ident.hpp"
#include "Value.hpp"
#include "Column.hpp"
#include "queries.hpp"

#include <charconv>
#include <parser.hpp>
#include <string>
#include <tuple>

using namespace parser;

// -------------------- Atoms --------------------
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

static is_parser_for<uint32_t> auto num =
    try_cast(
        view(seq(
            c('1', '9'),
            rep(c('0', '9'))
        )),
        [](auto str) -> std::optional<uint32_t> {
            uint32_t ans;

            auto res = std::from_chars(str.begin(), str.end(), ans);
            if (res.ec == std::errc {}) {
                return {ans};
            } else {
                return std::nullopt;
            }
        }
    );

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

static is_parser_for<int32_t> auto integer =
    cast(
        seq(
            neg(s("0x")),
            opt(any(c('+'), c('-'))), // sign
            num // num
        ),
        [](auto tup) -> int32_t {
            auto [sign, num] = std::move(tup);

            if (sign.has_value() && *sign == '-') {
                num = -num;
            }

            return num;
        }
    );

static is_parser_for<Value> auto value =
    any(
        cast(string,  [](auto v) { return Value::from_string(v); }),
        cast(bytes,   [](auto v) { return Value::from_bytes(v); }),
        cast(boolean, [](auto v) { return Value::from_bool(v); }),
        cast(integer, [](auto v) { return Value::from_int(v); })
    );

parser::Parser<Value> value_parser = value;

// -------------------- Ident --------------------

static is_parser_for<char> auto ident_first_char =
    any(c('a', 'z'), c('A', 'Z'), c('_'));

static is_parser_for<char> auto ident_char =
    any(ident_first_char, c('0', '9'));

static is_parser_for<Ident> auto ident =
    cast(
        view(seq(
            ident_first_char,
            rep(ident_char)
        )),
        [](auto s) { return Ident(std::string(s)); }
    );

parser::Parser<Ident> ident_parser = ident;

// -------------------- Type --------------------

static is_parser_for<uint32_t> auto type_size =
    seq(
        ws,
        ignore(c('[')),
        ws,
        num, // num
        ws,
        ignore(c(']'))
    );

static is_parser_for<ValueType> auto type =
    any(
        // int
        cast(
            seq(
                S("int"),
                opt(s("32"))
            ),
            [](auto) { return ValueType::INT; }
        ),
        // bool
        cast(
            S("bool"),
            [](auto) { return ValueType::BOOL; }
        ),
        // string
        cast(
            seq(
                S("str"),
                opt(S("ing")),
                opt(type_size)
            ),
            [](auto) { return ValueType::STRING; }
        ),
        // bytes 
        cast(
            seq(
                S("bytes"),
                opt(type_size)
            ),
            [](auto) { return ValueType::BYTES; }
        )
    );

parser::Parser<ValueType> type_parser = type;

// -------------------- Queries --------------------

// TODO: attributes
static is_parser_for<Column> auto column_def =
    cast(
        seq(
            ident, // name
            ws,
            ignore(c(':')),
            ws,
            type, // type
            opt(seq( // default_value
                ws,
                ignore(c('=')),
                ws,
                value
            ))
        ),
        [](auto tup) {
            auto [name, type, default_value] = std::move(tup);
            return Column(
                std::move(name),
                std::move(type),
                std::move(default_value)
            );
        }
    );

static is_parser_for<CreateTableQuery> auto create_table_query =
    cast(
        seq(
            ignore(S("create")),
            ws,
            ignore(S("table")),
            ws,
            ident, // name
            ws,
            ignore(c('(')),
            ws,
            rep(seq( // columns
                column_def,
                ignore(opt(seq(ws, c(','), ws)))
            )), 
            ws,
            ignore(c(')'))
        ),
        [](auto tup) {
            auto [name, columns] = std::move(tup);
            return CreateTableQuery {
                .table_name = std::move(name),
                .columns = std::move(columns),
            };
        }
    );

parser::Parser<CreateTableQuery> create_table_query_parser = create_table_query;
