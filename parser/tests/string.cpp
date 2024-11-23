#include <test_utils.hpp>

#include "parse.hpp"
#include "string.hpp"

using namespace parser;

TEST_GROUP(string)

TEST(case_sensitive, string)
    ASSERT_EQ(parse(s("abacaba"), "abacaba"), "abacaba");
    ASSERT_EQ(parse_opt(s("aBacaba"), "abacaba"), std::nullopt);
    ASSERT_EQ(parse_opt(s("abacaba"), "aBacaba"), std::nullopt);
END_TEST

TEST(ignore_case, string)
    ASSERT_EQ(parse(S("abacaba"), "abacaba"), "abacaba");
    ASSERT_EQ(parse(S("aBaCaBa"), "AbAcAba"), "AbAcAba");
END_TEST
