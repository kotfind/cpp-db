#include "Table.hpp"
#include "Expr.hpp"
#include "Row.hpp"
#include "Expr.hpp"

#include <cassert>
#include <stdexcept>
#include <utility>

Table::Table(Ident name_, std::vector<Column> columns_)
  : name(std::move(name_)),
    columns(std::move(columns_)),
    last_row_id(0)
{}

size_t Table::count_columns() const {
    return columns.size();
}
Column& Table::get_column(size_t num) {
    assert(num < count_columns());
    return columns.at(num);
}
Column& Table::get_column(const Ident& ident) {
    return columns.at(get_coulmn_num_by_name(ident));
}
const Column& Table::get_column(size_t num) const {
    assert(num < count_columns());
    return columns.at(num);
}
const Column& Table::get_column(const Ident& ident) const {
    return columns.at(get_coulmn_num_by_name(ident));
}

size_t Table::count_rows() const {
    return rows.size();
}
const Row* Table::get_row_by_id(size_t id) const {
    assert(rows.contains(id));
    return rows.at(id).get();
}
Row* Table::get_row_by_id(size_t id) {
    assert(rows.contains(id));
    return rows.at(id).get();
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

Row* Table::insert_row_named(RowInitializerNamed initializer) {
    auto row = std::unique_ptr<Row>(new Row(this, ++last_row_id, std::move(initializer)));
    auto* row_ptr = row.get();
    rows.insert({row->get_id(), std::move(row)});
    return row_ptr;
}

Row* Table::insert_row_positioned(RowInitializerPositioned initializer) {
    auto row = std::unique_ptr<Row>(new Row(this, ++last_row_id, std::move(initializer)));
    auto* row_ptr = row.get();
    rows.insert({row->get_id(), std::move(row)});
    return row_ptr;
}

std::vector<Row*> Table::select_rows(const Expr& cond) const {
    std::vector<Row*> ans;

    for (const auto& [_id, row] : rows) {
        // TODO: use indexes
        if (cond.eval(row->to_vars()).get_bool()) {
            ans.push_back(row.get());
        }
    }

    return ans;
}

std::vector<Row*> Table::update_rows(
    const std::unordered_map<Ident, Expr>& assignments,
    const Expr& cond
) {
    auto ans = Table::select_rows(cond);

    for (auto* row : ans) {
        for (const auto& [col, expr] : assignments) {
            (*row)[col] = expr.eval(row->to_vars());
        }
    }

    return ans;
}

size_t Table::delete_rows(const Expr& cond) {
    size_t rows_affected = 0;

    for (auto it = rows.begin(); it != rows.end();) {
        if (cond.eval(it->second->to_vars()).get_bool()) {
            ++rows_affected;
            it = rows.erase(it);
        } else {
            ++it;
        }
    }

    return rows_affected;
}

size_t Table::get_coulmn_num_by_name(const Ident& name) const {
    // TODO: optimize
    for (size_t col_id = 0; col_id < count_columns(); ++col_id) {
        if (get_column(col_id).get_name() == name) {
            return col_id;
        }
    }
    // Column not found
    throw std::runtime_error("column with name " + name.get_inner() + " not found");
}