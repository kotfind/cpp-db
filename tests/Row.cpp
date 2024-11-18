#include "Table.hpp"
#include "Value.hpp"
#include "Row.hpp"
#include "macro.hpp"
#include "test_utils.hpp"

TEST_GROUP(row)

Table get_empty_table() {
    Table table(Ident("people"), {
        Column(Ident("name"), ValueType::STRING),
        Column(Ident("is_male"), ValueType::BOOL, Value::from_bool(true)),
    });

    return table;
}

Table get_filled_table() {
    auto table = get_empty_table();

    table.push_row({{
        Value::from_string("Ivan"),
        Value::from_bool(true)
    }});

    return table;
}

TEST_GROUP(positioned_initializer, row)

    TEST(simple, positioned_initializer)
        auto table = get_empty_table();

        table.push_row({{
            Value::from_string("Ivan"),
            Value::from_bool(true)
        }});

        auto row = table.get_rows().back();

        ASSERT_EQ(row[0], Value::from_string("Ivan"));
        ASSERT_EQ(row[1], Value::from_bool(true));
    END_TEST

    TEST(with_default, positioned_initializer)
        auto table = get_empty_table();

        table.push_row({{
            Value::from_string("Ivan"),
            std::optional<Value>()
        }});

        auto row = table.get_rows().back();
        ASSERT_EQ(row[0], Value::from_string("Ivan"));
        ASSERT_EQ(row[1], Value::from_bool(true));
    END_TEST

    TEST(with_little_args, positioned_initializer)
        auto table = get_empty_table();

        table.push_row({{
            Value::from_string("Ivan"),
        }});

        auto row = table.get_rows().back();
        ASSERT_EQ(row[0], Value::from_string("Ivan"));
        ASSERT_EQ(row[1], Value::from_bool(true));
    END_TEST

TEST(size, row)
    auto table = get_filled_table();
    auto row = table.get_rows().back();

    ASSERT_EQ(row.size(), 2);
END_TEST

TEST(by_index, row)
    auto table = get_filled_table();
    auto row = table.get_rows().back();

    ASSERT_EQ(row[0], Value::from_string("Ivan"));
    ASSERT_EQ(row[1], Value::from_bool(true));
END_TEST

TEST(by_index_const, row)
    auto table = get_filled_table();
    const auto const_row = table.get_rows().back();

    ASSERT_EQ(const_row[0], Value::from_string("Ivan"));
    ASSERT_EQ(const_row[1], Value::from_bool(true));
END_TEST

TEST(get_data, row)
    auto table = get_filled_table();
    auto row = table.get_rows().back();

    std::vector<Value> data = {Value::from_string("Ivan"), Value::from_bool(true)};
    ASSERT_EQ(row.get_data(), data);
END_TEST

TEST(types, row)
    auto table = get_filled_table();
    auto types = table.get_rows().back().get_types();

    ASSERT_EQ(types[0], ValueType::STRING);
    ASSERT_EQ(types[1], ValueType::BOOL);
END_TEST
