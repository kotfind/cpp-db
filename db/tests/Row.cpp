#include <test_utils.hpp>

#include "Table.hpp"
#include "Value.hpp"
#include "Row.hpp"
#include "Column.hpp"

TEST_GROUP(row)

static Table get_empty_table() {
    Table table(Ident("people"), {
        Column(Ident("id"), ValueType::STRING, std::nullopt, COL_ATTR_AUTOINCREMENT),
        Column(Ident("name"), ValueType::STRING),
        Column(Ident("is_male"), ValueType::BOOL, Value::from_bool(true)),
    });

    return table;
}

static Table get_filled_table() {
    auto table = get_empty_table();

    table.insert_row_positioned({{
        std::nullopt,
        Value::from_string("Ivan"),
        Value::from_bool(true)
    }});

    return table;
}

TEST_GROUP(positioned_initializer, row)

    TEST(full, positioned_initializer)
        auto table = get_empty_table();

        table.insert_row_positioned({{
            std::nullopt,
            Value::from_string("Ivan"),
            Value::from_bool(true)
        }});

        auto& row = *table.get_row_by_id(1);

        ASSERT_EQ(row[0], Value::from_int(1));
        ASSERT_EQ(row[1], Value::from_string("Ivan"));
        ASSERT_EQ(row[2], Value::from_bool(true));
    END_TEST

    TEST(with_default, positioned_initializer)
        auto table = get_empty_table();

        table.insert_row_positioned({{
            std::nullopt,
            Value::from_string("Ivan"),
            std::optional<Value>()
        }});

        auto& row = *table.get_row_by_id(1);
        ASSERT_EQ(row[0], Value::from_int(1));
        ASSERT_EQ(row[1], Value::from_string("Ivan"));
        ASSERT_EQ(row[2], Value::from_bool(true));
    END_TEST

    TEST(with_little_args, positioned_initializer)
        auto table = get_empty_table();

        table.insert_row_positioned({{
            std::nullopt,
            Value::from_string("Ivan"),
        }});

        auto& row = *table.get_row_by_id(1);
        ASSERT_EQ(row[0], Value::from_int(1));
        ASSERT_EQ(row[1], Value::from_string("Ivan"));
        ASSERT_EQ(row[2], Value::from_bool(true));
    END_TEST

TEST_GROUP(named_initializer, row)

    TEST(full, named_initializer)
        auto table = get_empty_table();

        table.insert_row_named({{
            {Ident("name"), Value::from_string("Ivan")},
            {Ident("is_male"), Value::from_bool(true)},
        }});

        auto& row = *table.get_row_by_id(1);

        ASSERT_EQ(row[1], Value::from_string("Ivan"));
        ASSERT_EQ(row[2], Value::from_bool(true));
    END_TEST

    TEST(with_little_args, named_initializer)
        auto table = get_empty_table();

        table.insert_row_named({{
            {Ident("name"), Value::from_string("Ivan")},
        }});

        auto& row = *table.get_row_by_id(1);

        ASSERT_EQ(row[1], Value::from_string("Ivan"));
        ASSERT_EQ(row[2], Value::from_bool(true));
    END_TEST

TEST(size, row)
    auto table = get_filled_table();
    auto& row = *table.get_row_by_id(1);

    ASSERT_EQ(row.size(), 3);
END_TEST

TEST(by_index, row)
    auto table = get_filled_table();
    auto& row = *table.get_row_by_id(1);

    ASSERT_EQ(row[1], Value::from_string("Ivan"));
    ASSERT_EQ(row[2], Value::from_bool(true));
END_TEST

TEST(by_index_const, row)
    auto table = get_filled_table();
    const auto& const_row = *table.get_row_by_id(1);

    ASSERT_EQ(const_row[1], Value::from_string("Ivan"));
    ASSERT_EQ(const_row[2], Value::from_bool(true));
END_TEST

TEST(get_data, row)
    auto table = get_filled_table();
    auto& row = *table.get_row_by_id(1);

    std::vector<Value> data = {
        Value::from_int(1),
        Value::from_string("Ivan"),
        Value::from_bool(true)
    };
    ASSERT_EQ(row.get_data(), data);
END_TEST

TEST(types, row)
    auto table = get_filled_table();
    auto types = table.get_row_by_id(1)->get_types();

    ASSERT_EQ(types[1], ValueType::STRING);
    ASSERT_EQ(types[2], ValueType::BOOL);
END_TEST
