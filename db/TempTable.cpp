#include "TempTable.hpp"

#include "TempRow.hpp"
#include "TempColumn.hpp"

#include <cassert>
#include <memory>
#include <stdexcept>

TempTable::TempTable(std::vector<TempColumn> columns_)
  : columns(std::move(columns_))
{}

size_t TempTable::count_columns() const {
    return columns.size();
}

TempColumn& TempTable::get_column(size_t num) {
    assert(num < columns.size());
    return columns[num];
}

const TempColumn& TempTable::get_column(size_t num) const {
    assert(num < columns.size());
    return columns[num];
}

size_t TempTable::count_rows() const {
    return rows.size();
}

const TempRow* TempTable::get_row_by_num(size_t num) const {
    assert(num < rows.size());
    return rows[num].get();
}

TempRow* TempTable::get_row_by_num(size_t num) {
    assert(num < rows.size());
    return rows[num].get();
}

std::vector<ValueType> TempTable::get_types() const {
    std::vector<ValueType> ans;
    ans.reserve(columns.size());

    for (auto column : columns) {
        ans.push_back(column.get_type());
    }
    
    return ans;
}

void TempTable::insert_row(TempRow row) {
    if (row.get_types() != get_types()) {
        throw std::runtime_error("failed to insert_row: types of row and table don't match");
    }

    rows.push_back(std::make_unique<TempRow>(std::move(row)));
}
