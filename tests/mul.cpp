#include "mul.hpp"
#include "test_utils.hpp"

TEST_GROUP(mul_group)

    TEST(positive, mul_group)
        ASSERT_EQ(mul(2, 3), 6)
    END_TEST

    TEST(negative, mul_group)
        ASSERT_EQ(mul(-2, -3), 6)
    END_TEST
