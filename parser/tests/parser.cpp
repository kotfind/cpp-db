#include "test_utils.hpp"

#include "any.hpp"
#include "cast.hpp"
#include "ignore.hpp"
#include "sequence.hpp"
#include "string.hpp"

#include "parser.hpp"
#include "parse.hpp"
#include "char.hpp"

using namespace parser;

TEST_GROUP(parser)
    TEST(simple, parser)
        Parser<char> p = c('a', 'z');

        ASSERT_EQ(parse(p, "a"), 'a');
    END_TEST

    TEST(recursive, parser)
        Parser<size_t> braces;
        braces.initialize(any(
            cast(
                seq(
                    ignore(c('(')),
                    braces,
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
        ));

        ASSERT_EQ(parse(braces, ""), 0);
        ASSERT_EQ(parse(braces, "()"), 1);
        ASSERT_EQ(parse(braces, "((()))"), 3);
    END_TEST
