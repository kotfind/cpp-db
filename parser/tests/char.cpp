#include "test_utils.hpp"

#include "parse.hpp"
#include "char.hpp"

using namespace parser;

TEST_GROUP(char_)

TEST(range, char_)
    ASSERT_EQ(parse(c('a', 'z'), "n"), 'n');
    ASSERT_EQ(parse(c('a', 'z'), "m"), 'm');
    ASSERT_EQ(parse_opt(c('a', 'z'), "A"), std::nullopt);
END_TEST

TEST(exact, char_)
    ASSERT_EQ(parse(c('a'), "a"), 'a');
    ASSERT_EQ(parse_opt(c('a'), "m"), std::nullopt);
END_TEST

TEST(any, char_)
    ASSERT_EQ(parse(c(), "a"), 'a');
    ASSERT_EQ(parse(c(), "b"), 'b');
END_TEST

