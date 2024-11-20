#include "parser.hpp"

#include <charconv>
#include <optional>
#include <system_error>

PARSER(num_p)
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
    )
END_PARSER
