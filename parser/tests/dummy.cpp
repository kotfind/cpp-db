#include <test_utils.hpp>

#include "parse.hpp"
#include "dummy.hpp"
#include "char.hpp"
#include "sequence.hpp"
#include <tuple>

using namespace parser;

TEST(dummy)
    auto p = seq(
        c('a'),
        dummy(),
        dummy(),
        dummy(),
        dummy(),
        c('b')
    );
    ASSERT_EQ(parse(p, "ab"), std::make_tuple('a', 'b'));
END_TEST
