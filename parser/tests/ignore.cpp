#include "test_utils.hpp"

#include "ignore.hpp"
#include "char.hpp"
#include "parse.hpp"

#include <optional>
#include <variant>

using namespace parser;

TEST(ignore_)
    auto p = ignore(c('a', 'z'));

    ASSERT_EQ(parse(p, "a"), std::monostate());
    ASSERT_EQ(parse_opt(p, "A"), std::nullopt);
END_TEST
