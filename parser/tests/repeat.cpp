#include "test_utils.hpp"

#include "char.hpp"
#include "seq.hpp"
#include "repeat.hpp"
#include "parse.hpp"

TEST_GROUP(repeat)

TEST(range, repeat)
    auto p = seq(
        rep(c('a', 'z'), 3, 5),
        c('X')
    );
    using type = decltype(p)::type;

    ASSERT_EQ(parse_opt(p, "abX"), std::nullopt);

    {
        type ans{{'a', 'b', 'c'}, 'X'};
        ASSERT_EQ(parse(p, "abcX"), ans);
    }

    {
        type ans{{'a', 'b', 'c', 'd'}, 'X'};
        ASSERT_EQ(parse(p, "abcdX"), ans);
    }

    {
        type ans{{'a', 'b', 'c', 'd', 'e'}, 'X'};
        ASSERT_EQ(parse(p, "abcdeX"), ans);
    }

    ASSERT_EQ(parse_opt(p, "abcdefX"), std::nullopt);
END_TEST

TEST(no_more_than, repeat)
    // Check that parser won't "eat" too much
    auto p = seq(
        rep(c('a', 'z'), 0, 2),
        c('a', 'z')
    );
    using type = decltype(p)::type;

    type ans{{'a', 'b'}, 'c'};
    ASSERT_EQ(parse(p, "abc"), ans);
END_TEST

TEST(exact, repeat)
    auto p = seq(
        rep(c('a', 'z'), 3),
        c('X')
    );
    using type = decltype(p)::type;

    ASSERT_EQ(parse_opt(p, "abX"), std::nullopt);

    {
        type ans{{'a', 'b', 'c'}, 'X'};
        ASSERT_EQ(parse(p, "abcX"), ans);
    }

    ASSERT_EQ(parse_opt(p, "abcdX"), std::nullopt);
END_TEST

TEST(zero_or_more, repeat)
    auto p = seq(
        rep(c('a', 'z')),
        c('X')
    );
    using type = decltype(p)::type;

    {
        type ans{{}, 'X'};
        ASSERT_EQ(parse(p, "X"), ans);
    }

    {
        type ans{{'a'}, 'X'};
        ASSERT_EQ(parse(p, "aX"), ans);
    }

    {
        type ans{{'a', 'b'}, 'X'};
        ASSERT_EQ(parse(p, "abX"), ans);
    }

    {
        type ans{{'a', 'b', 'c'}, 'X'};
        ASSERT_EQ(parse(p, "abcX"), ans);
    }
END_TEST

TEST(one_or_more, repeat)
    auto p = seq(
        rep1(c('a', 'z')),
        c('X')
    );
    using type = decltype(p)::type;

    ASSERT_EQ(parse_opt(p, "X"), std::nullopt);

    {
        type ans{{'a'}, 'X'};
        ASSERT_EQ(parse(p, "aX"), ans);
    }

    {
        type ans{{'a', 'b'}, 'X'};
        ASSERT_EQ(parse(p, "abX"), ans);
    }

    {
        type ans{{'a', 'b', 'c'}, 'X'};
        ASSERT_EQ(parse(p, "abcX"), ans);
    }
END_TEST

TEST(rep_min, repeat)
    auto p = seq(
        rep_min(c('a', 'z'), 3),
        c('X')
    );
    using type = decltype(p)::type;

    ASSERT_EQ(parse_opt(p, "abX"), std::nullopt);

    {
        type ans{{'a', 'b', 'c'}, 'X'};
        ASSERT_EQ(parse(p, "abcX"), ans);
    }

    {
        type ans{{'a', 'b', 'c', 'd'}, 'X'};
        ASSERT_EQ(parse(p, "abcdX"), ans);
    }

END_TEST

TEST(rep_max, repeat)
    auto p = seq(
        rep_max(c('a', 'z'), 5),
        c('X')
    );
    using type = decltype(p)::type;

    {
        type ans{{'a', 'b', 'c'}, 'X'};
        ASSERT_EQ(parse(p, "abcX"), ans);
    }

    {
        type ans{{'a', 'b', 'c', 'd'}, 'X'};
        ASSERT_EQ(parse(p, "abcdX"), ans);
    }

    {
        type ans{{'a', 'b', 'c', 'd', 'e'}, 'X'};
        ASSERT_EQ(parse(p, "abcdeX"), ans);
    }

    ASSERT_EQ(parse_opt(p, "abcdefX"), std::nullopt);
END_TEST