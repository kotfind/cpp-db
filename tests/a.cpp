#include "test_utils/test_utils.hpp"

TEST(x)
    return true;
END_TEST

TEST_GROUP(a)

    TEST(a1, a)
    END_TEST

    TEST(a2, a)
        return false;
    END_TEST

TEST_GROUP(b)

    TEST(b1, b)
        return true;
    END_TEST

    TEST(b2, b)
        return false;
    END_TEST
