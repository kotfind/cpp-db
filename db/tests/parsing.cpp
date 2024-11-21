#include "test_utils.hpp"

#include "Column.hpp"
#include "parsing.hpp"

#include <cstring>

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

TEST(ident, parsing)
    ASSERT_EQ(parse(ident_parser, "hello_world_123"), Ident("hello_world_123"));
END_TEST

TEST(type, parsing)
    ASSERT_EQ(parse(type_parser, "inT"), ValueType::INT);
    ASSERT_EQ(parse(type_parser, "INt32"), ValueType::INT);

    ASSERT_EQ(parse(type_parser, "BoOl"), ValueType::BOOL);

    ASSERT_EQ(parse(type_parser, "stR"), ValueType::STRING);
    ASSERT_EQ(parse(type_parser, "sTrING"), ValueType::STRING);
    ASSERT_EQ(parse(type_parser, "string [ 123]"), ValueType::STRING);
    ASSERT_EQ(parse(type_parser, "str[ 123]"), ValueType::STRING);

    ASSERT_EQ(parse(type_parser, "bytEs"), ValueType::BYTES);
    ASSERT_EQ(parse(type_parser, "ByTes [ 123]"), ValueType::BYTES);
END_TEST

TEST_GROUP(queries, parsing)

    TEST(create_table_query, queries)
        auto str =
            R"(CREATE TABLE people (
                name : STR,
                age:INT = 18
            ))";
        auto parsed = parse(create_table_query_parser, str);

        ASSERT_EQ(parsed.table_name, Ident("people"));
        ASSERT_EQ(parsed.columns[0], Column(Ident("name"), ValueType::STRING, std::nullopt));
        ASSERT_EQ(parsed.columns[1], Column(Ident("age"), ValueType::INT, {Value::from_int(18)}));
    END_TEST

    TEST(insert_query, queries)
        auto str =
            R"(INSERT (
                name = "Ivan",
                age = 20
            ) TO people)";

        auto parsed = parse(insert_query_parser, str);

        ASSERT_EQ(parsed.table_name, Ident("people"));

        auto* row = std::get_if<RowInitializerNamed>(&parsed.row);
        ASSERT(row != nullptr);

        ASSERT_EQ(row->get_values().at(Ident("name")), Value::from_string("Ivan"));
        ASSERT_EQ(row->get_values().at(Ident("age")), Value::from_int(20));
    END_TEST
