#include "Table.hpp"
#include "Row.hpp"

#include <cassert>
#include <utility>

Table::Table(Ident name, std::vector<Column> columns)
  : name(std::move(name)),
    columns(std::move(columns)),
    last_row_id(0)
{}

const std::vector<Column>& Table::get_columns() const {
    return columns;
}

std::vector<Column>& Table::get_columns() {
    return columns;
}

const std::vector<std::unique_ptr<Row>>& Table::get_rows() const {
    return rows;
}

std::vector<std::unique_ptr<Row>>& Table::get_rows() {
    return rows;
}

std::vector<ValueType> Table::get_types() const {
    std::vector<ValueType> ans;
    ans.reserve(columns.size());

    for (auto column : columns) {
        ans.push_back(column.get_type());
    }
    
    return ans;
}

const Ident& Table::get_name() const {
    return name;
}

void Table::push_row_named(RowInitializerNamed initializer) {
    rows.push_back(std::unique_ptr<Row>(new Row(this, ++last_row_id, std::move(initializer))));
}

void Table::push_row_positioned(RowInitializerPositioned initializer) {
    rows.push_back(std::unique_ptr<Row>(new Row(this, ++last_row_id, std::move(initializer))));
}
