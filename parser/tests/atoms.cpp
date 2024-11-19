#include "test_utils.hpp"

#include "atoms.hpp"
#include "parse.hpp"

TEST_GROUP(atoms)

TEST_GROUP(char_, atoms)

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

TEST_GROUP(string, atoms)

    TEST(case_sensitive, string)
        ASSERT_EQ(parse(s("abacaba"), "abacaba"), "abacaba");
        ASSERT_EQ(parse_opt(s("aBacaba"), "abacaba"), std::nullopt);
        ASSERT_EQ(parse_opt(s("abacaba"), "aBacaba"), std::nullopt);
    END_TEST

    TEST(ignore_case, string)
        ASSERT_EQ(parse(S("abacaba"), "abacaba"), "abacaba");
        ASSERT_EQ(parse(S("aBaCaBa"), "AbAcAba"), "AbAcAba");
    END_TEST
