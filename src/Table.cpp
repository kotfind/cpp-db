#include "Table.hpp"

#include <cassert>

Table::Table(Ident name, std::vector<Column> columns)
  : name(std::move(name)),
    columns(std::move(columns))
{}

void Table::push_row(Row row) {
    assert(row.get_types() == get_types());
    rows.push_back(std::move(row));
}

const std::vector<Column>& Table::get_columns() const {
    return columns;
}

std::vector<Column>& Table::get_columns() {
    return columns;
}

const std::vector<Row>& Table::get_rows() const {
    return rows;
}

std::vector<Row>& Table::get_rows() {
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
