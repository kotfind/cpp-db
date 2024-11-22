#include "test_utils.hpp"

#include "TempTable.hpp"
#include "Value.hpp"
#include "TempRow.hpp"
#include "TempColumn.hpp"

TEST_GROUP(temp_row)

static TempTable get_empty_table() {
    TempTable table({
        TempColumn(std::string("name"), ValueType::STRING),
        TempColumn(std::string("age < 10"), ValueType::BOOL),
    });

    return table;
}

static TempTable get_filled_table() {
    auto table = get_empty_table();

    table.insert_row({{}, {
        Value::from_string("Ivan"),
        Value::from_bool(true)
    }});

    return table;
}

TEST(init, temp_row)
    auto table = get_empty_table();

    // NOTE: for testings purposes only.
    // Nullptr should not be a parent_row
    Row* parent_row = nullptr;

    table.insert_row({{parent_row}, {
        Value::from_string("Ivan"),
        Value::from_bool(true)
    }});

    auto& row = *table.get_row_by_num(0);

    ASSERT_EQ(row[0], Value::from_string("Ivan"));
    ASSERT_EQ(row[1], Value::from_bool(true));

    ASSERT_EQ(row.get_parent_rows()[0], nullptr);
END_TEST

TEST(size, temp_row)
    auto table = get_filled_table();
    auto& row = *table.get_row_by_num(0);

    ASSERT_EQ(row.size(), 2);
END_TEST

TEST(by_index, temp_row)
    auto table = get_filled_table();
    auto& row = *table.get_row_by_num(0);

    ASSERT_EQ(row[0], Value::from_string("Ivan"));
    ASSERT_EQ(row[1], Value::from_bool(true));
END_TEST

TEST(by_index_const, temp_row)
    auto table = get_filled_table();
    const auto& const_row = *table.get_row_by_num(0);

    ASSERT_EQ(const_row[0], Value::from_string("Ivan"));
    ASSERT_EQ(const_row[1], Value::from_bool(true));
END_TEST

TEST(get_data, temp_row)
    auto table = get_filled_table();
    auto& row = *table.get_row_by_num(0);

    std::vector<Value> data = {Value::from_string("Ivan"), Value::from_bool(true)};
    ASSERT_EQ(row.get_data(), data);
END_TEST

TEST(types, temp_row)
    auto table = get_filled_table();
    auto types = table.get_row_by_num(0)->get_types();

    ASSERT_EQ(types[0], ValueType::STRING);
    ASSERT_EQ(types[1], ValueType::BOOL);
END_TEST
