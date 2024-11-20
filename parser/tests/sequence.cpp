#include "any.hpp"
#include "ignore.hpp"
#include "test_utils.hpp"

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

TEST(ignore_empty, seq)
    auto ws = ignore(rep(any(
        c('\n'),
        c(' '),
        c('\t'),
        c('\r')
    )));

    auto p = seq(
        s("hello"),
        ws, // returns std::tuple<>, is not included in seq's return type
        s("world")
    );

    using type = decltype(p)::type;
    type ans{"hello", "world"};

    ASSERT_EQ(parse(p, "helloworld"), ans);
    ASSERT_EQ(parse(p, "hello\n \t world"), ans);
END_TEST
