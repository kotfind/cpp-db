#include "parser.hpp"

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
}
