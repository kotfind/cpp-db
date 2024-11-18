#include "Expr.hpp"
#include <cassert>
#include <memory>
#include <stdexcept>
#include <variant>

BinaryExpr::BinaryExpr(BinaryExprOp op_, std::unique_ptr<Expr> lhs_, std::unique_ptr<Expr> rhs_)
  : op(op_),
    lhs(std::move(lhs_)),
    rhs(std::move(rhs_))
{}

Value BinaryExpr::eval(const VarMap& vars) const {
    auto lhs = this->lhs->eval(vars);
    auto rhs = this->rhs->eval(vars);

    switch (op) {
        case BinaryExprOp::ADD:
            return lhs + rhs;

        case BinaryExprOp::SUB:
            return lhs - rhs;

        case BinaryExprOp::MUL:
            return lhs * rhs;

        case BinaryExprOp::DIV:
            return lhs / rhs;

        case BinaryExprOp::MOD:
            return lhs % rhs;

        case BinaryExprOp::LE:
            return Value::from_bool(lhs < rhs);

        case BinaryExprOp::EQ:
            return Value::from_bool(lhs == rhs);

        case BinaryExprOp::GR:
            return Value::from_bool(lhs > rhs);

        case BinaryExprOp::LEQ:
            return Value::from_bool(lhs <= rhs);

        case BinaryExprOp::GEQ:
            return Value::from_bool(lhs >= rhs);

        case BinaryExprOp::NEQ:
            return Value::from_bool(lhs != rhs);

        case BinaryExprOp::AND:
            return Value::from_bool(lhs && rhs);

        case BinaryExprOp::OR:
            return Value::from_bool(lhs || rhs);

        case BinaryExprOp::XOR:
            return Value::from_bool(lhs ^ rhs);

        default:
            // Unreachable
            assert(false);
    }
}

UnaryExpr::UnaryExpr(UnaryExprOp op_, std::unique_ptr<Expr> arg_)
  : op(op_),
    arg(std::move(arg_))
{}

Value UnaryExpr::eval(const VarMap& vars) const {
    auto arg = this->arg->eval(vars);
    switch (op) {
        case UnaryExprOp::MINUS:
            return -arg;

        case UnaryExprOp::NOT:
            return Value::from_bool(!arg);

        case UnaryExprOp::LEN:
            if (arg.is_string()) {
                return Value::from_int(arg.get_string().size());
            } else if (arg.is_bytes()) {
                return Value::from_int(arg.get_bytes().size());
            } else {
                throw std::runtime_error("cannot get length of value, that is neither string nor bytes");
            }

        default:
            // Unreachable
            assert(false);
    }
}

Expr::Expr(BinaryExpr expr)
  : inner(std::move(expr))
{}

Expr::Expr(UnaryExpr expr)
  : inner(std::move(expr))
{}

Expr::Expr(Value value)
  : inner(std::move(value))
{}

Expr::Expr(Ident ident)
  : inner(std::move(ident))
{}

Value Expr::eval(const VarMap& vars) const {
    if (auto* binary = std::get_if<BinaryExpr>(&inner)) {
        return binary->eval(vars);
    } else if (auto* unary = std::get_if<UnaryExpr>(&inner)) {
        return unary->eval(vars);
    } else if (auto* value = std::get_if<Value>(&inner)) {
        return *value;
    } else if (auto* ident = std::get_if<Ident>(&inner)) {
        if (!vars.contains(*ident)) {
            throw std::runtime_error("undefined identifier " + ident->get_inner());
        }
        return vars.at(*ident);
    } else {
        // Unreachable
        assert(false);
    }
}
