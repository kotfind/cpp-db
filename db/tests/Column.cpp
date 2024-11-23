#include <test_utils.hpp>

#include "Column.hpp"
#include "Value.hpp"

TEST_GROUP(column)

TEST(simple, column)
    Column col(Ident("id"), ValueType::INT);
    ASSERT_EQ(col.get_name(), Ident("id"));
    ASSERT_EQ(col.get_type(), ValueType::INT);
END_TEST

TEST(default_value, column)
    Column col(Ident("age"), ValueType::INT, Value::from_int(18));
    ASSERT_EQ(*col.get_default_value_ext(1), Value::from_int(18));
END_TEST

TEST(eq, column)
    Column col1(Ident("id"), ValueType::INT);
    Column col2(Ident("d"), ValueType::STRING);
    Column col3(Ident("age"), ValueType::INT);
    ASSERT(col1 == col1);
    ASSERT(col1 != col2);
    ASSERT(col1 != col3);
END_TEST
