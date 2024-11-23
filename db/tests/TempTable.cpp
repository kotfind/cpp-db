#include <test_utils.hpp>

#include "TempTable.hpp"
#include "TempColumn.hpp"
#include "TempRow.hpp"
#include "Value.hpp"

TEST_GROUP(temp_table)

static TempTable get_empty_table() {
    TempTable table({
        TempColumn(std::string("name"), ValueType::STRING),
        TempColumn(std::string("is_male"), ValueType::BOOL),
    });
    return table;
}

TEST(get_columns, temp_table)
    auto table = get_empty_table();

    ASSERT_EQ(table.get_column(0), TempColumn(std::string("name"), ValueType::STRING));
    ASSERT_EQ(table.get_column(1), TempColumn(std::string("is_male"), ValueType::BOOL));
END_TEST

TEST(get_types, temp_table)
    auto types = get_empty_table().get_types();

    ASSERT_EQ(types[0], ValueType::STRING);
    ASSERT_EQ(types[1], ValueType::BOOL);
END_TEST

static TempTable get_filled_table() {
    TempTable table({
        TempColumn(std::string("name"), ValueType::STRING),
        TempColumn(std::string("is_male"), ValueType::BOOL),
    });

    table.insert_row({{}, {Value::from_string("Ivan"), Value::from_bool(true)}});
    table.insert_row({{}, {Value::from_string("Ann"), Value::from_bool(false)}});
    table.insert_row({{}, {Value::from_string("Jim"), Value::from_bool(true)}});

    return table;
}

TEST(rows, temp_table)
    auto table = get_filled_table();
    ASSERT_EQ((*table.get_row_by_num(0))[0], Value::from_string("Ivan"));
    ASSERT_EQ((*table.get_row_by_num(0))[1], Value::from_bool(true));

    ASSERT_EQ((*table.get_row_by_num(1))[0], Value::from_string("Ann"));
    ASSERT_EQ((*table.get_row_by_num(1))[1], Value::from_bool(false));

    ASSERT_EQ((*table.get_row_by_num(2))[0], Value::from_string("Jim"));
    ASSERT_EQ((*table.get_row_by_num(2))[1], Value::from_bool(true));
END_TEST
