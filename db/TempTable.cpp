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

std::ostream& operator<<(std::ostream& out, const TempTable& table) {
    if (table.count_rows() == 0) {
        out << "EMPTY\n";
    }

    for (size_t col_num = 0; col_num < table.count_columns(); ++col_num) {
        if (col_num != 0) {
            out << " | ";
        }
        auto col = table.get_column(col_num);
        out << col.get_name() << " (" << col.get_type() << ")";
    }
    out << '\n';

    for (size_t row_num = 0; row_num < table.count_rows(); ++row_num) {
        auto& row = *table.get_row_by_num(row_num);
        for (size_t i = 0; i < row.size(); ++i) {
            if (i != 0) {
                out << " | ";
            }
            out << row[i];
        }
        out << '\n';
    }

    return out;
}
