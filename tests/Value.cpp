#include "test_utils.hpp"

#include "Value.hpp"

TEST_GROUP(value)

TEST(get_type, value)
    ASSERT_EQ(Value::from_int(1).get_type(), ValueType::INT);
    ASSERT_EQ(Value::from_bool(true).get_type(), ValueType::BOOL);
    ASSERT_EQ(Value::from_string("Hello, world!").get_type(), ValueType::STRING);
    ASSERT_EQ(Value::from_bytes({std::byte{1}, std::byte{2}, std::byte{3}}).get_type(), ValueType::BYTES);
END_TEST

TEST(is, value)
    ASSERT_EQ(Value::from_int(1).is_int(), true);
    ASSERT_EQ(Value::from_bool(true).is_bool(), true);
    ASSERT_EQ(Value::from_string("Hello, world!").is_string(), true);
    ASSERT_EQ(Value::from_bytes({std::byte{1}, std::byte{2}, std::byte{3}}).is_bytes(), true);

    ASSERT_EQ(Value::from_int(1).is_bool(), false);
    ASSERT_EQ(Value::from_bool(true).is_string(), false);
    ASSERT_EQ(Value::from_string("Hello, world!").is_bytes(), false);
    ASSERT_EQ(Value::from_bytes({std::byte{1}, std::byte{2}, std::byte{3}}).is_int(), false);
END_TEST

TEST(get, value)
    ASSERT_EQ(Value::from_int(1).get_int(), 1);
    ASSERT_EQ(Value::from_bool(true).get_bool(), true);
    ASSERT_EQ(Value::from_string("Hello, world!").get_string(), "Hello, world!");
    std::vector<std::byte> bytes{std::byte{1}, std::byte{2}, std::byte{3}};
    ASSERT_EQ(Value::from_bytes(bytes).get_bytes(), bytes);
END_TEST

TEST(try_get, value)
    ASSERT_EQ(*Value::from_int(1).try_get_int(), 1);
    ASSERT_EQ(*Value::from_bool(true).try_get_bool(), true);
    ASSERT_EQ(*Value::from_string("Hello, world!").try_get_string(), "Hello, world!");
    std::vector<std::byte> bytes{std::byte{1}, std::byte{2}, std::byte{3}};
    ASSERT_EQ(*Value::from_bytes(bytes).try_get_bytes(), bytes);

    ASSERT_EQ(Value::from_int(1).try_get_bool(), nullptr);
    ASSERT_EQ(Value::from_bool(true).try_get_string(), nullptr);
    ASSERT_EQ(Value::from_string("Hello, world!").try_get_bytes(), nullptr);
    ASSERT_EQ(Value::from_bytes(bytes).try_get_int(), nullptr);
END_TEST
