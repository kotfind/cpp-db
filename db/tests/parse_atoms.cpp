#include "test_utils.hpp"

#include "parse_atoms.hpp"

#include <cstddef>

using namespace parse_atoms;

TEST_GROUP(parse_atoms)

TEST(ws, parse_atoms)
    std::vector<std::string> strs = {
        "   ",
        " \n \t ",
        "/* hello */",
        "  \n/* he\nllo */ ",
        "  \n// hello\n /* world */",
    };

    parse(ws(), "");

    for (const auto& str : strs) {
        parse(ws(), str);
        parse(ws1(), str);
    }
END_TEST

TEST(quoted_string, parse_atoms)
    ASSERT_EQ(parse(string(), R"("abacaba")"), "abacaba");
    ASSERT_EQ(parse(string(), R"("a\n\na\r\"c\taba")"), "a\n\na\r\"c\taba");
END_TEST

TEST(boolean, parse_atoms)
    ASSERT_EQ(parse(boolean(), "TrUe"), true);
    ASSERT_EQ(parse(boolean(), "FalSE"), false);
END_TEST

TEST(bytes, parse_atoms)
    {
        std::vector<std::byte> ans{std::byte{0x1a}, std::byte{0xbc}};
        ASSERT_EQ(parse(bytes(), "0x1abc"), ans);
    }

    {
        std::vector<std::byte> ans{std::byte{0x02}, std::byte{0x1a}, std::byte{0xbc}};
        ASSERT_EQ(parse(bytes(), "0x21abc"), ans);
    }
END_TEST

TEST(num, parse_atoms)
    ASSERT_EQ(parse(num(), "42"), 42);
    ASSERT_EQ(parse(num(), "-42"), -42);
    ASSERT_EQ(parse(num(), "+42"), +42);
END_TEST
