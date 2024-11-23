#include "test_utils.hpp"
#include <iostream>
#include <sstream>

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

TEST(eq, value)
    ASSERT(Value::from_int(1) == Value::from_int(1));
    ASSERT(Value::from_int(1) != Value::from_int(2));

    ASSERT(Value::from_bool(true) == Value::from_bool(true));
    ASSERT(Value::from_bool(true) != Value::from_bool(false));

    ASSERT(Value::from_string("Hello, world!") == Value::from_string("Hello, world!"));
    ASSERT(Value::from_string("Hello, world!") != Value::from_string("Hello, C++!"));

    std::vector<std::byte> bytes1{std::byte{1}, std::byte{2}, std::byte{3}};
    std::vector<std::byte> bytes2{std::byte{1}, std::byte{2}};
    ASSERT(Value::from_bytes(bytes1) == Value::from_bytes(bytes1));
    ASSERT(Value::from_bytes(bytes1) != Value::from_bytes(bytes2));
END_TEST


TEST_GROUP(cmp, value)
    TEST(le, cmp)
        ASSERT_EQ(Value::from_int(1) < Value::from_int(2), true);
        ASSERT_EQ(Value::from_int(2) < Value::from_int(2), false);
        ASSERT_EQ(Value::from_int(2) < Value::from_int(1), false);
    END_TEST

    TEST(gr, cmp)
        ASSERT_EQ(Value::from_int(1) > Value::from_int(2), false);
        ASSERT_EQ(Value::from_int(2) > Value::from_int(2), false);
        ASSERT_EQ(Value::from_int(2) > Value::from_int(1), true);
    END_TEST

    TEST(leq, cmp)
        ASSERT_EQ(Value::from_int(1) <= Value::from_int(2), true);
        ASSERT_EQ(Value::from_int(2) <= Value::from_int(2), true);
        ASSERT_EQ(Value::from_int(2) <= Value::from_int(1), false);
    END_TEST

    TEST(geq, cmp)
        ASSERT_EQ(Value::from_int(1) >= Value::from_int(2), false);
        ASSERT_EQ(Value::from_int(2) >= Value::from_int(2), true);
        ASSERT_EQ(Value::from_int(2) >= Value::from_int(1), true);
    END_TEST

    TEST(eq, cmp)
        ASSERT_EQ(Value::from_int(1) == Value::from_int(2), false);
        ASSERT_EQ(Value::from_int(2) == Value::from_int(2), true);
    END_TEST

    TEST(neq, cmp)
        ASSERT_EQ(Value::from_int(1) != Value::from_int(2), true);
        ASSERT_EQ(Value::from_int(2) != Value::from_int(2), false);
    END_TEST

TEST_GROUP(math, value)

    TEST(add, math)
        ASSERT_EQ(Value::from_int(1) + Value::from_int(2), Value::from_int(3));
        ASSERT_EQ(Value::from_string("aba") + Value::from_string("caba"), Value::from_string("abacaba"));
    END_TEST

    TEST(sub, math)
        ASSERT_EQ(Value::from_int(1) - Value::from_int(2), Value::from_int(-1));
    END_TEST

    TEST(mul, math)
        ASSERT_EQ(Value::from_int(6) * Value::from_int(7), Value::from_int(42));
    END_TEST

    TEST(div, math)
        ASSERT_EQ(Value::from_int(42) / Value::from_int(7), Value::from_int(6));
    END_TEST

    TEST(mod, math)
        ASSERT_EQ(Value::from_int(12) / Value::from_int(5), Value::from_int(2));
    END_TEST

    TEST(minus, math)
        ASSERT_EQ(-Value::from_int(1), Value::from_int(-1));
    END_TEST

TEST_GROUP(boolean, value)

    TEST(or, boolean)
        ASSERT_EQ(Value::from_bool(false) || Value::from_bool(false), false);
        ASSERT_EQ(Value::from_bool( true) || Value::from_bool(false),  true);
        ASSERT_EQ(Value::from_bool(false) || Value::from_bool( true),  true);
        ASSERT_EQ(Value::from_bool( true) || Value::from_bool( true),  true);
    END_TEST

    TEST(and, boolean)
        ASSERT_EQ(Value::from_bool(false) && Value::from_bool(false), false);
        ASSERT_EQ(Value::from_bool( true) && Value::from_bool(false), false);
        ASSERT_EQ(Value::from_bool(false) && Value::from_bool( true), false);
        ASSERT_EQ(Value::from_bool( true) && Value::from_bool( true),  true);
    END_TEST

    TEST(xor, boolean)
        ASSERT_EQ(Value::from_bool(false) ^ Value::from_bool(false), false);
        ASSERT_EQ(Value::from_bool( true) ^ Value::from_bool(false),  true);
        ASSERT_EQ(Value::from_bool(false) ^ Value::from_bool( true),  true);
        ASSERT_EQ(Value::from_bool( true) ^ Value::from_bool( true), false);
    END_TEST

    TEST(not_, boolean)
        ASSERT_EQ(!Value::from_bool(false),  true);
        ASSERT_EQ(!Value::from_bool( true), false);
    END_TEST

template<typename T>
std::string to_string(const T& x) {
    std::stringstream ss;
    ss << x;
    return ss.str();
}

TEST(print, value)
    ASSERT_EQ(to_string(Value::from_string("hello\nworld")), R"("hello\nworld")");
    ASSERT_EQ(to_string(Value::from_int(-42)), "-42");
    ASSERT_EQ(to_string(Value::from_bool(true)), "true");

    std::vector<std::byte> bytes = {std::byte {0xab}, std::byte{0xcd}};
    ASSERT_EQ(to_string(Value::from_bytes(bytes)), "0xabcd");
END_TEST
