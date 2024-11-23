#include "any.hpp"
#include "ignore.hpp"
#include <test_utils.hpp>

#include "parse.hpp"
#include "sequence.hpp"
#include "char.hpp"
#include "string.hpp"
#include "view.hpp"
#include "forward.hpp"
#include "repeat.hpp"

#include <tuple>
#include <iostream>

using namespace parser;

TEST_GROUP(seq)

TEST(simple, seq)
    auto p = seq(s("Hello"), c('A', 'Z'), s("world!"));
    ASSERT_EQ(parse(p, "HelloXworld!"), std::make_tuple("Hello", 'X', "world!"));
    ASSERT_EQ(parse(p, "HelloAworld!"), std::make_tuple("Hello", 'A', "world!"));
END_TEST

static auto ws = ignore(rep(any(
    c('\n'),
    c(' '),
    c('\t'),
    c('\r')
)));

TEST(some_empty, seq)
     // ws's return std::tuple<>, so they are not included in seq's return type
    auto p = seq(
        ws,
        s("hello"),
        ws,
        s("world"),
        ws
    );

    using type = decltype(p)::type;
    type ans{"hello", "world"};

    ASSERT_EQ(parse(p, "helloworld"), ans);
    ASSERT_EQ(parse(p, "hello\n \t world"), ans);
    ASSERT_EQ(parse(p, " hello\tworld\n"), ans);
END_TEST

TEST(all_empty, seq)
    auto p = seq(
        ignore(s("hello")),
        ws,
        ignore(s("world"))
    );

    std::tuple<> ans{};

    ASSERT_EQ(parse(p, "hello  world"), ans);
END_TEST

TEST(single, seq)
    auto p = seq(
        ws,
        s("hello"),
        ws
    );

    // Note: result has type std::string_view, NOT std::tuple<std::string_view>

    ASSERT_EQ(parse(p, "\nhello\n"), "hello");
END_TEST
