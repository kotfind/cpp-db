#include "macro.hpp"
#include "test_utils.hpp"
#include "Table.hpp"
#include "Value.hpp"

TEST_GROUP(table)

Table get_sample_table() {
    Table table(Ident("people"), {
        Column(Ident("id"), ValueType::INT),
        Column(Ident("name"), ValueType::STRING),
        Column(Ident("is_male"), ValueType::BOOL),
    });
    return table;
}

TEST(name, table)
    ASSERT_EQ(get_sample_table().get_name(), Ident("people"));
END_TEST

TEST(get_columns, table)
    auto columns = get_sample_table().get_columns();

    ASSERT_EQ(columns[0], Column(Ident("id"), ValueType::INT));
    ASSERT_EQ(columns[1], Column(Ident("name"), ValueType::STRING));
    ASSERT_EQ(columns[2], Column(Ident("is_male"), ValueType::BOOL));
END_TEST

TEST(get_types, table)
    auto types = get_sample_table().get_types();

    ASSERT_EQ(types[0], ValueType::INT);
    ASSERT_EQ(types[1], ValueType::STRING);
    ASSERT_EQ(types[2], ValueType::BOOL);
END_TEST

TEST(rows, table)
    auto table = get_sample_table();

    Row row1{{Value::from_int(1), Value::from_string("Ivan"), Value::from_bool(true)}};
    Row row2{{Value::from_int(2), Value::from_string("Ann"), Value::from_bool(false)}};
    Row row3{{Value::from_int(3), Value::from_string("Jim"), Value::from_bool(true)}};

    table.push_row(row1);
    table.push_row(row2);
    table.push_row(row3);

    auto rows = table.get_rows();
    ASSERT_EQ(rows[0], row1);
    ASSERT_EQ(rows[1], row2);
    ASSERT_EQ(rows[2], row3);
END_TEST
