#include "test_utils.hpp"

#include "sequence.hpp"
#include "option.hpp"
#include "any.hpp"
#include "char.hpp"
#include "repeat.hpp"
#include "view.hpp"
#include "cast.hpp"
#include "parser.hpp"

#include <string>

using namespace parser;

// Note: this example is good for demonstration, but bad for production
// as it copies string_view and don't check for cast failure.
// Check try_cast.cpp for secure but scary example.
TEST(cast)
    auto num_p =
        cast(
            view(seq(
                opt(any(c('+'), c('-'))),
                c('1', '9'),
                rep(c('0', '9'))
            )),
            [](auto str) {
                std::string s(str);
                return std::stoi(s);
            }
        );

    ASSERT_EQ(parse(num_p, "42"), 42);
    ASSERT_EQ(parse(num_p, "-42"), -42);
    ASSERT_EQ(parse(num_p, "+42"), +42);
END_TEST
