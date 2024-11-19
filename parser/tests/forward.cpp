#include "test_utils.hpp"

#include "char.hpp"
#include "forward.hpp"
#include "parse.hpp"
#include "seq.hpp"

TEST(forward)
    auto p = seq(fwd(c('a')), c('a', 'x'));
    using type = decltype(p)::type;

    type ans = {'a', 'a'};
    ASSERT_EQ(parse(p, "a"), ans);

    ASSERT_EQ(parse_opt(p, "x"), std::nullopt);
END_TEST