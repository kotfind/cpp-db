#pragma once

#include <parser.hpp>

#include "Ident.hpp"
#include "Value.hpp"

extern parser::Parser<Value> value_parser;

extern parser::Parser<ValueType> type_parser;

extern parser::Parser<Ident> ident_parser;
