#include "test_utils.hpp"

#include "parse.hpp"
#include "seq.hpp"
#include "char.hpp"
#include "string.hpp"

#include <tuple>

TEST(seq_)
    auto p = seq(s("Hello"), c('A', 'Z'), s("world!"));
    ASSERT_EQ(parse(p, "HelloXworld!"), std::make_tuple("Hello", 'X', "world!"));
    ASSERT_EQ(parse(p, "HelloAworld!"), std::make_tuple("Hello", 'A', "world!"));
END_TEST
