#pragma once

#include "Expr.hpp"
#include "Ident.hpp"
#include "RowInitializerNamed.hpp"
#include "RowInitializerPositioned.hpp"

#include <vector>

class Column;

struct CreateTableQuery {
    const Ident table_name;
    const std::vector<Column> columns;
};

struct DropTableQuery {
    const Ident table_name;
};

struct SelectQuery {
    // TODO: select expressions
    const Ident table_name;
    const Expr cond;
};

struct InsertQuery {
    const std::variant<RowInitializerNamed, RowInitializerPositioned> row;
    const Ident table_name;
};

struct UpdateQuery {
    const Ident table_name;
    const std::unordered_map<Ident, Expr>& assignments;
    const Expr cond;
};

struct DeleteQuery {
    const Ident table_name;
    const Expr cond;
};
