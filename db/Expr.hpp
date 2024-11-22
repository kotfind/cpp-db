#pragma once

#include <memory>
#include <unordered_map>
#include <variant>

#include "Ident.hpp"
#include "Value.hpp"

using VarMap = std::unordered_map<Ident, Value>;

class Expr;

enum class BinaryExprOp {
    ADD, // +
    SUB, // -
    MUL, // *
    DIV, // /
    MOD, // %
    LE,  // <
    EQ,  // =
    GR,  // >
    LEQ, // <=
    GEQ, // >=
    NEQ, // !=
    AND, // &&
    OR,  // ||
    XOR, // ^^
};

class BinaryExpr {
    public:
        BinaryExpr(BinaryExprOp op, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs);
        BinaryExpr(BinaryExprOp op, Expr lhs, Expr rhs);
        BinaryExpr(const BinaryExpr& other);

        BinaryExpr& operator=(const BinaryExpr& other);

        Value eval(const VarMap& vars) const;

    private:
        BinaryExprOp op;
        std::unique_ptr<Expr> lhs;
        std::unique_ptr<Expr> rhs;
};

enum class UnaryExprOp {
    MINUS, // -
    NOT,   // !
    LEN,   // |x|
};

class UnaryExpr {
    public:
        UnaryExpr(UnaryExprOp op, std::unique_ptr<Expr> arg);
        UnaryExpr(UnaryExprOp op, Expr arg);
        UnaryExpr(const UnaryExpr& other);

        UnaryExpr& operator=(const UnaryExpr& other);

        Value eval(const VarMap& vars) const;

    private:
        UnaryExprOp op;
        std::unique_ptr<Expr> arg;
};

class Expr {
    public:
        Expr(BinaryExpr expr);
        Expr(UnaryExpr expr);
        Expr(Value value);
        Expr(Ident ident);

        Value eval(const VarMap& vars) const;

    private:
        std::variant<
            BinaryExpr,
            UnaryExpr,
            Value,
            Ident
        > inner;
};


Expr operator<(Expr lhs, Expr rhs);
Expr operator>(Expr lhs, Expr rhs);
Expr operator<=(Expr lhs, Expr rhs);
Expr operator>=(Expr lhs, Expr rhs);
Expr operator==(Expr lhs, Expr rhs);
Expr operator!=(Expr lhs, Expr rhs);

Expr operator+(Expr lhs, Expr rhs);
Expr operator-(Expr lhs, Expr rhs);
Expr operator*(Expr lhs, Expr rhs);
Expr operator/(Expr lhs, Expr rhs);
Expr operator%(Expr lhs, Expr rhs);
Expr operator-(Expr arg);

Expr operator|(Expr lhs, Expr rhs);
Expr operator&(Expr lhs, Expr rhs);
Expr operator^(Expr lhs, Expr rhs);
Expr operator!(Expr arg);

Expr len(Expr arg);
