#include "Expr.hpp"
#include "test_utils.hpp"

#include "Column.hpp"
#include "parsing.hpp"

#include <cstring>
#include <iostream>
#include <optional>

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

TEST_GROUP(expr, parsing)

    TEST(simple, expr)
        ASSERT_EQ(
            parse(expr_parser, "2 + 3 * 4").eval(VarMap {}),
            Value::from_int(14)
        );
    END_TEST

    TEST(precedence, expr)
        ASSERT(is_same(
            parse(expr_parser, "a || b && c ^^ d == e > f + g - h * (i + !j)"),
            Ident("a") || Ident("b") && Ident("c") ^ Ident("d") == Ident("e") >
            Ident("f") + Ident("g") - Ident("h") * (Ident("i") + !Ident("j"))
        ));
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

    TEST(drop_table_query, queries)
        auto str =
            R"(DROP TABLE people)";
        auto parsed = parse(drop_table_query_parser, str);
        ASSERT_EQ(parsed.table_name, Ident("people"));
    END_TEST

    TEST(insert_query_named, queries)
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

    TEST(insert_query_positioned, queries)
        auto str =
            R"(INSERT ("Ivan",) TO people)";

        auto parsed = parse(insert_query_parser, str);

        ASSERT_EQ(parsed.table_name, Ident("people"));

        auto* row = std::get_if<RowInitializerPositioned>(&parsed.row);
        ASSERT(row != nullptr);

        ASSERT_EQ(row->get_values()[0], Value::from_string("Ivan"));
        ASSERT_EQ(row->get_values()[1], std::nullopt);
    END_TEST

    TEST(select_query, queries)
        auto str =
            R"(SELECT
                name,
                age,
            FROM people WHERE age > 18 || name == "Ivan")";

        auto parsed = parse(select_query_parser, str);

        // TODO: check fields

        ASSERT_EQ(parsed.table_name, Ident("people"));
        ASSERT(is_same(
            parsed.cond,
            Ident("age") > Value::from_int(18) || Ident("name") == Value::from_string("Ivan")
        ));
    END_TEST

    TEST(update_query, queries)
        auto str =
            R"(UPDATE people
            SET
                name = "Peter",
                age = 2 * age - 10,
            WHERE
                name == "Ivan")";

        auto parsed = parse(update_query_parser, str);

        // TODO: check fields

        ASSERT_EQ(parsed.table_name, Ident("people"));

        ASSERT(is_same(
            parsed.cond,
            Ident("name") == Value::from_string("Ivan")
        ));

        ASSERT(is_same(
            parsed.assignments.at(Ident("name")),
            Value::from_string("Peter")
        ));

        ASSERT(is_same(
            parsed.assignments.at(Ident("age")),
            Value::from_int(2) * Ident("age") - Value::from_int(10)
        ));
    END_TEST

    TEST(delete_query, queries)
        auto str =
            R"(DELETE
                people
            WHERE
                name == "Ivan")";

        auto parsed = parse(delete_query_parser, str);

        ASSERT_EQ(parsed.table_name, Ident("people"));

        ASSERT(is_same(
            parsed.cond,
            Ident("name") == Value::from_string("Ivan")
        ));
    END_TEST

    TEST(no_where_clause, queries)
        auto str =
            R"(DELETE people)";

        auto parsed = parse(delete_query_parser, str);

        ASSERT_EQ(parsed.table_name, Ident("people"));

        ASSERT(is_same(
            parsed.cond,
            Value::from_bool(true)
        ));
    END_TEST

    TEST(queries, queries)
        auto str = R"(
            CREATE TABLE users (
                name: /* some comment */ string[32],
                pass: bytes[8],
                is_admin: bool = false,
            );

            INSERT ("Ivan", 0xa1c,) TO users;

            INSERT (name = "Jack", pass = 0xaaa, is_admin = true) TO users;

            SELECT name, |name|, pass
            FROM users
            WHERE is_admin == true;

            UPDATE users
            SET
                is_admin = !is_admin
            WHERE
                name == "Ivan";

            DELETE
            FROM users
            WHERE name == "Jack";

            DROP TABLE users;
        )";

        parse(queries_parser, str);
    END_TEST
