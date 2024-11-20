#include "test_utils.hpp"

#include "parse_atoms.hpp"

TEST(num_p_)
    ASSERT_EQ(parse(num_p(), "42"), 42);
    ASSERT_EQ(parse(num_p(), "-42"), -42);
    ASSERT_EQ(parse(num_p(), "+42"), +42);
END_TEST
