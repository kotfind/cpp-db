#include "test_utils.hpp"

#include "char.hpp"
#include "any.hpp"
#include "parser.hpp"

using namespace parser;

TEST(any)
    auto p = any(c('A', 'Z'), c('a', 'z'), c('0', '9'));
    ASSERT_EQ(parse(p, "a"), 'a');
    ASSERT_EQ(parse(p, "A"), 'A');
    ASSERT_EQ(parse(p, "0"), '0');
END_TEST
