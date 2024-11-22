#pragma once

#include "Expr.hpp"
#include "Ident.hpp"
#include "RowInitializerNamed.hpp"
#include "RowInitializerPositioned.hpp"
#include "TempTable.hpp"

#include <tuple>
#include <variant>

class Column;
class TempTable;

struct CreateTableQuery {
    const Ident table_name;
    const std::vector<Column> columns;
};

struct DropTableQuery {
    const Ident table_name;
};

struct SelectQuery {
    const std::vector<Expr> exprs;
    const Ident table_name;
    const Expr cond;
};

struct InsertQuery {
    const std::variant<RowInitializerNamed, RowInitializerPositioned> row;
    const Ident table_name;
};

struct UpdateQuery {
    const Ident table_name;
    const std::unordered_map<Ident, Expr> assignments;
    const Expr cond;
};

struct DeleteQuery {
    const Ident table_name;
    const Expr cond;
};

using AnyQuery = std::variant<
    CreateTableQuery,
    DropTableQuery,
    SelectQuery,
    InsertQuery,
    UpdateQuery,
    DeleteQuery
>;

struct QueryResult : std::variant<
    TempTable,   // Rows
    size_t,      // Rows affected
    std::tuple<> // Nothing
> {};
