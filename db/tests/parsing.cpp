#include "test_utils.hpp"

#include "parsing.hpp"

TEST_GROUP(parsing)

TEST_GROUP(value, parsing)

    TEST(quoted_string, value)
        ASSERT_EQ(
            parse(value_parser, R"("abacaba")"),
            Value::from_string("abacaba")
        );

        ASSERT_EQ(
            parse(value_parser, R"("a\n\na\r\"c\taba")"),
            Value::from_string("a\n\na\r\"c\taba")
        );
    END_TEST

    TEST(boolean, value)
        ASSERT_EQ(parse(value_parser, "TrUe"), Value::from_bool(true));
        ASSERT_EQ(parse(value_parser, "FalSE"), Value::from_bool(false));
    END_TEST

    TEST(bytes, value)
        {
            std::vector<std::byte> ans{std::byte{0x1a}, std::byte{0xbc}};
            ASSERT_EQ(parse(value_parser, "0x1abc"), Value::from_bytes(ans));
        }

        {
            std::vector<std::byte> ans{std::byte{0x02}, std::byte{0x1a}, std::byte{0xbc}};
            ASSERT_EQ(parse(value_parser, "0x21abc"), Value::from_bytes(ans));
        }
    END_TEST

    TEST(num, value)
        ASSERT_EQ(parse(value_parser, "42"), Value::from_int(42));
        ASSERT_EQ(parse(value_parser, "-42"), Value::from_int(-42));
        ASSERT_EQ(parse(value_parser, "+42"), Value::from_int(+42));
    END_TEST
