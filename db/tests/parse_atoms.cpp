#include "test_utils.hpp"

#include "parse_atoms.hpp"

#include <cstddef>

using namespace parse_atoms;

TEST_GROUP(parse_atoms)

TEST(ws, parse_atoms)
    std::vector<std::string> strs = {
        "   ",
        " \n \t ",
        "/* hello */",
        "  \n/* he\nllo */ ",
        "  \n// hello\n /* world */",
    };

    parse(ws(), "");

    for (const auto& str : strs) {
        parse(ws(), str);
        parse(ws1(), str);
    }
END_TEST
