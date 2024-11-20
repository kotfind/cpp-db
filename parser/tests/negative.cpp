#include "test_utils.hpp"

#include "char.hpp"
#include "negative.hpp"
#include "parse.hpp"
#include "sequence.hpp"

using namespace parser;

TEST(negative)
    auto p = seq(neg(c('a')), c('a', 'x'));
    using type = decltype(p)::type;

    ASSERT_EQ(parse_opt(p, "a"), std::nullopt);

    type ans = {std::monostate {}, 'x'};
    ASSERT_EQ(parse(p, "x"), ans);
END_TEST
