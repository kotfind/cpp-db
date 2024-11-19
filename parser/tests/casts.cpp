#include "test_utils.hpp"

#include "seq.hpp"
#include "option.hpp"
#include "any.hpp"
#include "char.hpp"
#include "repeat.hpp"
#include "view.hpp"
#include "cast.hpp"
#include "parse.hpp"

#include <string>

TEST(cast_)
    auto num_p =
        cast(
            view(seq(
                opt(any(c('+'), c('-'))),
                c('1', '9'),
                rep(c('0', '9'))
            )),
            [](auto str) -> int {
                // using try_cast and from_chars here would be better
                std::string s(str);
                return std::stoi(s);
            }
        );

    ASSERT_EQ(parse(num_p, "42"), 42);
    ASSERT_EQ(parse(num_p, "-42"), -42);
    ASSERT_EQ(parse(num_p, "+42"), +42);
END_TEST
