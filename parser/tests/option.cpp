#include "test_utils.hpp"

#include "option.hpp"
#include "char.hpp"
#include "parser.hpp"
#include "sequence.hpp"

using namespace parser;

TEST(option)
    auto p = seq(opt(c('a')), c('x'));
    using type = decltype(p)::type;

    {
        type ans{{'a'}, 'x'};
        ASSERT_EQ(parse(p, "ax"), ans);
    }

    {
        type ans{{}, 'x'};
        ASSERT_EQ(parse(p, "x"), ans);
    }
END_TEST
