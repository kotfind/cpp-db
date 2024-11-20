#include "test_utils.hpp"

#include "parser.hpp"
#include "char.hpp"

using namespace parser;

TEST(parser)
    Parser<char> p = c('a', 'z');

    ASSERT_EQ(parse(p, "a"), 'a');
END_TEST
