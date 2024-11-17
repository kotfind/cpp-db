#include "Value.hpp"
#include "Row.hpp"
#include "macro.hpp"
#include "test_utils.hpp"

TEST_GROUP(row)

Row get_sample_row() {
    Row row{{Value::from_int(1), Value::from_string("Ivan"), Value::from_bool(true)}};
    return row;
}

TEST(size, row)
    ASSERT_EQ(get_sample_row().size(), 3);
END_TEST

TEST(by_index, row)
    auto row = get_sample_row();
    ASSERT_EQ(row[0], Value::from_int(1));
    ASSERT_EQ(row[1], Value::from_string("Ivan"));
    ASSERT_EQ(row[2], Value::from_bool(true));
END_TEST

TEST(by_index_const, row)
    const auto row = get_sample_row();
    ASSERT_EQ(row[0], Value::from_int(1));
    ASSERT_EQ(row[1], Value::from_string("Ivan"));
    ASSERT_EQ(row[2], Value::from_bool(true));
END_TEST

TEST(get_data, row)
    std::vector<Value> data{Value::from_int(1), Value::from_string("Ivan"), Value::from_bool(true)};
    Row row{data};
    ASSERT_EQ(row.get_data(), data);
END_TEST

TEST(types, row)
    auto types = get_sample_row().get_types();
    ASSERT_EQ(types[0], ValueType::INT);
    ASSERT_EQ(types[1], ValueType::STRING);
    ASSERT_EQ(types[2], ValueType::BOOL);
END_TEST

TEST(eq, row)
    Row r1{{Value::from_int(1), Value::from_string("Ivan"), Value::from_bool(true)}};
    Row r2{{Value::from_int(2), Value::from_string("Ann"), Value::from_bool(false)}};
    ASSERT(r1 == r1);
    ASSERT(r1 != r2);
END_TEST
