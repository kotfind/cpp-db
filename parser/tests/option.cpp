#include "test_utils.hpp"

#include "option.hpp"
#include "char.hpp"
#include "parse.hpp"
#include "seq.hpp"

using namespace parser;

TEST(option_)
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
