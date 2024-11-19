#include "test_utils.hpp"

#include "char.hpp"
#include "parse.hpp"
#include "repeat.hpp"
#include "view.hpp"

using namespace parser;

TEST(view_)
    ASSERT_EQ(parse(view(rep(c('a', 'z'))), "abacaba"), "abacaba");
END_TEST
