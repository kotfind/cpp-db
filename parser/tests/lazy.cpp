#include <test_utils.hpp>

#include "cast.hpp"
#include "char.hpp"
#include "lazy.hpp"
#include "ignore.hpp"
#include "parse.hpp"
#include "parser.hpp"
#include "any.hpp"
#include "string.hpp"
#include "sequence.hpp"

using namespace parser;

Parser<size_t> braces() {
    static Parser<size_t> parser =
        any(
            cast(
                seq(
                    ignore(c('(')),
                    lazy(braces), // num
                    ignore(c(')'))
                ),
                [](auto num) -> size_t {
                    return num + 1;
                }
            ),
            cast(
                s(""),
                [](auto) -> size_t { return 0; }
            )
        );
    return parser;
}

TEST(lazy)
    ASSERT_EQ(parse(braces(), ""), 0);
    ASSERT_EQ(parse(braces(), "()"), 1);
    ASSERT_EQ(parse(braces(), "((()))"), 3);
END_TEST
