#include "macro.hpp"
#include "test_utils.hpp"
#include "Column.hpp"
#include "Value.hpp"

TEST_GROUP(column)

TEST(simple, column)
    Column col(Ident("id"), ValueType::INT);
    ASSERT_EQ(col.get_name(), Ident("id"));
    ASSERT_EQ(col.get_type(), ValueType::INT);
END_TEST
