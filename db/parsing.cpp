#include "parsing.hpp"

#include "Ident.hpp"
#include "Value.hpp"
#include "Column.hpp"
#include "queries.hpp"
#include "Expr.hpp"

#include <charconv>
#include <parser.hpp>
#include <string>
#include <tuple>
#include <cassert>

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

template<is_parser P>
static is_parser_for<std::vector<typename P::type>> auto csv(P parser) {
    return seq(
        betw(
            std::move(parser),
            seq(ws, c(','), ws)
        ),
        ignore(opt(seq(ws, c(','))))
    );
}

template<is_parser P>
static is_parser_for<typename P::type> auto braced(P parser) {
    return seq(
        ignore(c('(')),
        ws,
        std::move(parser),
        ws,
        ignore(c(')'))
    );
}

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

// -------------------- Expr --------------------

static Parser<Expr> expr();

static Parser<Expr> expr_prec_0() {
    static Parser<Expr> p = any(
        // len
        cast(
            seq(
                ignore(c('|')),
                ws,
                lazy(expr), // expr
                ws,
                ignore(c('|'))
            ),
            [](auto expr) -> Expr {
                return len(std::move(expr));
            }
        ),
        // braced
        braced(lazy(expr)),
        // value
        cast(value, [](auto v) -> Expr { return {v}; }),
        // ident
        cast(ident, [](auto i) -> Expr { return {i}; }),
        // unary operator
        cast(
            seq(
                any(c('+'), c('-'), c('!')), // op
                ws,
                lazy(expr) // expr
            ),
            [](auto tup) {
                auto [op, expr] = std::move(tup);

                switch (op) {
                    case '+': return std::move(expr);
                    case '-': return -std::move(expr);
                    case '!': return !std::move(expr);
                    default:  assert(false);
                }
            }
        )
    );
    return p;
}

static Parser<Expr> expr_prec_1() {
    static Parser<Expr> p = cast(
        seq(
            lazy(expr_prec_0), // expr
            rep(seq( // exprs
                ws,
                any(c('*'), c('/'), c('%')), // exprs.op
                ws,
                lazy(expr_prec_0) // exprs.other_expr
            ))
        ),
        [](auto tup) -> Expr {
            auto [expr, exprs] = std::move(tup);
            for (auto& [op, other_expr] : exprs) {
                switch (op) {
                    case '*':
                        expr = std::move(expr) * std::move(other_expr);
                        break;

                    case '/':
                        expr = std::move(expr) / std::move(other_expr);
                        break;

                    case '%':
                        expr = std::move(expr) % std::move(other_expr);
                        break;

                    default:
                        assert(false);
                }
            }
            return std::move(expr);
        }
    );
    return p;
}

static Parser<Expr> expr_prec_2() {
    static Parser<Expr> p = cast(
        seq(
            lazy(expr_prec_1), // expr
            rep(seq( // exprs
                ws,
                any(c('+'), c('-')), // exprs.op
                ws,
                lazy(expr_prec_1) // exprs.other_expr
            ))
        ),
        [](auto tup) -> Expr {
            auto [expr, exprs] = std::move(tup);
            for (auto& [op, other_expr] : exprs) {
                switch (op) {
                    case '+':
                        expr = std::move(expr) + std::move(other_expr);
                        break;

                    case '-':
                        expr = std::move(expr) - std::move(other_expr);
                        break;

                    default:
                        assert(false);
                }
            }
            return std::move(expr);
        }
    );
    return p;
}

static Parser<Expr> expr_prec_3() {
    static Parser<Expr> p = cast(
        seq(
            lazy(expr_prec_2), // expr
            opt(seq( // exprs
                ws,
                any(s("<"), s(">"), s(">="), s("<=")), // exprs.op
                ws,
                lazy(expr_prec_2) // exprs.other_expr
            ))
        ),
        [](auto tup) -> Expr {
            auto [expr, exprs] = std::move(tup);
            if (!exprs.has_value()) {
                return expr;
            }
            auto [op, other_expr] = std::move(*exprs);
            if (op == "<") {
                return std::move(expr) < std::move(other_expr);
            } else if (op == ">") {
                return std::move(expr) > std::move(other_expr);
            } else if (op == "<=") {
                return std::move(expr) <= std::move(other_expr);
            } else if (op == ">=") {
                return std::move(expr) >= std::move(other_expr);
            } else {
                assert(false);
            }
        }
    );
    return p;
}

static Parser<Expr> expr_prec_4() {
    static Parser<Expr> p = cast(
        seq(
            lazy(expr_prec_3), // expr
            opt(seq( // exprs
                ws,
                any(s("=="), s("!=")), // exprs.op
                ws,
                lazy(expr_prec_3) // exprs.other_expr
            ))
        ),
        [](auto tup) -> Expr {
            auto [expr, exprs] = std::move(tup);
            if (!exprs.has_value()) {
                return expr;
            }
            auto [op, other_expr] = std::move(*exprs);
            if (op == "==") {
                return std::move(expr) == std::move(other_expr);
            } else if (op == "!=") {
                return std::move(expr) != std::move(other_expr);
            } else {
                assert(false);
            }
        }
    );
    return p;
}

static Parser<Expr> expr_prec_5() {
    static Parser<Expr> p = cast(
        seq(
            lazy(expr_prec_4), // expr
            rep(seq( // exprs
                ws,
                s("^^"), // exprs.op
                ws,
                lazy(expr_prec_4) // exprs.other_expr
            ))
        ),
        [](auto tup) -> Expr {
            auto [expr, exprs] = std::move(tup);
            for (auto& [op, other_expr] : exprs) {
                expr = std::move(expr) ^ std::move(other_expr);
            }
            return std::move(expr);
        }
    );
    return p;
}

static Parser<Expr> expr_prec_6() {
    static Parser<Expr> p = cast(
        seq(
            lazy(expr_prec_5), // expr
            rep(seq( // exprs
                ws,
                s("&&"), // exprs.op
                ws,
                lazy(expr_prec_5) // exprs.other_expr
            ))
        ),
        [](auto tup) -> Expr {
            auto [expr, exprs] = std::move(tup);
            for (auto& [op, other_expr] : exprs) {
                expr = std::move(expr) && std::move(other_expr);
            }
            return std::move(expr);
        }
    );
    return p;
}

static Parser<Expr> expr_prec_7() {
    static Parser<Expr> p = cast(
        seq(
            lazy(expr_prec_6), // expr
            rep(seq( // exprs
                ws,
                s("||"), // exprs.op
                ws,
                lazy(expr_prec_6) // exprs.other_expr
            ))
        ),
        [](auto tup) -> Expr {
            auto [expr, exprs] = std::move(tup);
            for (auto& [op, other_expr] : exprs) {
                expr = std::move(expr) || std::move(other_expr);
            }
            return std::move(expr);
        }
    );
    return p;
}

Parser<Expr> expr() {
    return expr_prec_7();
}

Parser<Expr> expr_parser = expr();

// -------------------- Create Table Query --------------------

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
            braced(csv(column_def)) // columns
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

// -------------------- Insert Query --------------------

static is_parser_for<RowInitializerNamed> auto row_initializer_named =
    cast(
        braced(csv(seq(
            ident, // name
            ws,
            ignore(c('=')),
            ws,
            value // value
        ))),
        [](auto value_defs) {
            std::unordered_map<Ident, Value> values;
            values.reserve(value_defs.size());
            for (auto& value_def : value_defs) {
                auto [name, value] = std::move(value_def);
                values.insert({name, value});
            }
            return RowInitializerNamed(std::move(values));
        }
    );

static is_parser_for<RowInitializerPositioned> auto row_initializer_positioned =
    cast(
        braced(csv(opt(value))),
        [](auto value_defs) {
            return RowInitializerPositioned(std::move(value_defs));
        }
    );

using RowInitializerVariant = std::variant<RowInitializerNamed, RowInitializerPositioned>;

static is_parser_for<RowInitializerVariant> auto row_initializer =
    any(
        cast(row_initializer_positioned, [](auto v) { return RowInitializerVariant{v}; }),
        cast(row_initializer_named,      [](auto v) { return RowInitializerVariant{v}; })
    );

static is_parser_for<InsertQuery> auto insert_query =
    cast(
        seq(
            ignore(S("insert")),
            ws,
            row_initializer, // row
            ws,
            ignore(S("to")),
            ws,
            ident // table_name
        ),
        [](auto tup) {
            auto [row, table_name] = std::move(tup);

            return InsertQuery {
                .row = std::move(row),
                .table_name = std::move(table_name),
            };
        }
    );
parser::Parser<InsertQuery> insert_query_parser = insert_query;

// -------------------- WHERE --------------------


static is_parser_for<Expr> auto opt_where_clause =
    cast(
        opt(seq( // cond
            ws,
            ignore(S("where")),
            ws,
            lazy(expr) // opt_expr
        )),
        [](auto opt_expr) {
            
            return opt_expr.has_value()
                ? std::move(*opt_expr)
                : Expr(Value::from_bool(true));
        }
    );

// -------------------- Select Query --------------------

static is_parser_for<SelectQuery> auto select_query = 
    cast(
        seq(
            ignore(S("select")),
            ws,
            csv(seq(neg(S("from")), ident)), // column_names
            ws,
            ignore(S("from")),
            ws,
            ident, // table_name
            ws,
            opt_where_clause // cond
        ),
        [](auto tup) {
            auto [column_names, table_name, cond] = std::move(tup);
            // TODO: don't ignore column_names
            return SelectQuery {
                .table_name = std::move(table_name),
                .cond = std::move(cond)
            };
        }
    );
parser::Parser<SelectQuery> select_query_parser = select_query;

// -------------------- Update Query --------------------
static is_parser_for<UpdateQuery> auto update_query = 
    cast(
        seq(
            ignore(S("update")),
            ws,
            ident, // table_name
            ws,
            ignore(S("set")),
            ws,
            csv(seq( // assignment_defs
                ident, // name
                ws,
                ignore(c('=')),
                ws,
                lazy(expr) // value
            )),
            ws,
            opt_where_clause // cond
        ),
        [](auto tup) {
            auto [table_name, assignment_defs, cond] = std::move(tup);

            std::unordered_map<Ident, Expr> assignments;
            for (auto& [name, value] : assignment_defs) {
                assignments.insert({std::move(name), std::move(value)});
            }

            return UpdateQuery {
                .table_name = std::move(table_name),
                .assignments = std::move(assignments),
                .cond = std::move(cond),
            };
        }
    );
parser::Parser<UpdateQuery> update_query_parser = update_query;

// -------------------- Delete Query --------------------
static is_parser_for<DeleteQuery> auto delete_query = 
    cast(
        seq(
            ignore(S("delete")),
            ws,
            ident, // table_name
            ws,
            opt_where_clause // cond
        ),
        [](auto tup) {
            auto [table_name, cond] = std::move(tup);

            return DeleteQuery {
                .table_name = std::move(table_name),
                .cond = std::move(cond),
            };
        }
    );
parser::Parser<DeleteQuery> delete_query_parser = delete_query;
