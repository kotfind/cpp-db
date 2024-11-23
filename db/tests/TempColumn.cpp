#include <test_utils.hpp>

#include "TempColumn.hpp"
#include "Value.hpp"

TEST_GROUP(temp_column)

TEST(simple, temp_column)
    TempColumn col(std::string("a + b"), ValueType::INT);
    ASSERT_EQ(col.get_name(), std::string("a + b"));
    ASSERT_EQ(col.get_type(), ValueType::INT);
END_TEST

TEST(eq, temp_column)
    TempColumn col1(std::string("id"), ValueType::INT);
    TempColumn col2(std::string("d"), ValueType::STRING);
    TempColumn col3(std::string("age"), ValueType::INT);
    ASSERT(col1 == col1);
    ASSERT(col1 != col2);
    ASSERT(col1 != col3);
END_TEST
