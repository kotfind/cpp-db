#pragma once

#include <parser.hpp>

#include "Ident.hpp"
#include "Value.hpp"
#include "queries.hpp"

extern parser::Parser<Value> value_parser;
extern parser::Parser<ValueType> type_parser;

extern parser::Parser<Ident> ident_parser;

extern parser::Parser<Expr> expr_parser;

extern parser::Parser<CreateTableQuery> create_table_query_parser;
extern parser::Parser<InsertQuery> insert_query_parser;
extern parser::Parser<SelectQuery> select_query_parser;
