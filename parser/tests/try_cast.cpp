#include "test_utils.hpp"

#include "sequence.hpp"
#include "option.hpp"
#include "any.hpp"
#include "char.hpp"
#include "repeat.hpp"
#include "view.hpp"
#include "try_cast.hpp"
#include "parse.hpp"

#include <charconv>
#include <system_error>

using namespace parser;

// Note: this example is pretty scary
// check cast.cpp for less scary (but less secure) example
TEST(try_cast)
    auto num_p =
        try_cast(
            view(seq(
                opt(any(c('+'), c('-'))),
                c('1', '9'),
                rep(c('0', '9'))
            )),
            [](auto str) -> std::optional<int> {
                int ans;

                // from_chars refuses to parse leading +
                if (str.starts_with('+')) {
                    str.remove_prefix(1);
                }

                auto res = std::from_chars(str.begin(), str.end(), ans);
                if (res.ec == std::errc {}) {
                    return {ans};
                } else {
                    return std::nullopt;
                }
            }
        );

    ASSERT_EQ(parse(num_p, "42"), 42);
    ASSERT_EQ(parse(num_p, "-42"), -42);
    ASSERT_EQ(parse(num_p, "+42"), +42);

    // Overflows int
    ASSERT_EQ(parse_opt(num_p, "11111111111"), std::nullopt);
END_TEST
