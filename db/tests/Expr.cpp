#include <test_utils.hpp>

#include "Expr.hpp"

TEST_GROUP(expr)

TEST(eval, expr)
    // |s + "caba"| * 2 = v - 4
    // s = "aba"
    // v = 18
    auto expr = len(Ident("s") + Value::from_string("caba")) * Value::from_int(2) == Ident("v") - Value::from_int(4);

    VarMap vars {
        { Ident("s"), Value::from_string("aba") },
        { Ident("v"), Value::from_int(18) },
    };

    ASSERT_EQ(expr.eval(vars), Value::from_bool(true));
END_TEST
