#include "Expr.hpp"
#include "macro.hpp"
#include "test_utils.hpp"

TEST_GROUP(expr)

TEST(eval, expr)
    // |s + "caba"| * 2 = v - 4
    // s = "aba"
    // v = 18
    Expr expr = BinaryExpr(
        BinaryExprOp::EQ,
        std::make_unique<Expr>(BinaryExpr(
            BinaryExprOp::MUL,
            std::make_unique<Expr>(UnaryExpr(
                UnaryExprOp::LEN,
                std::make_unique<Expr>(BinaryExpr(
                    BinaryExprOp::ADD,
                    std::make_unique<Expr>(Ident("s")),
                    std::make_unique<Expr>(Value::from_string("caba"))
                ))
            )),
            std::make_unique<Expr>(Value::from_int(2))
        )),
        std::make_unique<Expr>(BinaryExpr(
            BinaryExprOp::SUB,
            std::make_unique<Expr>(Ident("v")),
            std::make_unique<Expr>(Value::from_int(4))
        ))
    );

    VarMap vars {
        { Ident("s"), Value::from_string("aba") },
        { Ident("v"), Value::from_int(18) },
    };

    ASSERT_EQ(expr.eval(vars), Value::from_bool(true));
END_TEST
