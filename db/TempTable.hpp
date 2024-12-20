#pragma once

#include "Value.hpp"
#include "TempRow.hpp"
#include "TempColumn.hpp"

#include <memory>
#include <ostream>
#include <vector>

class Expr;

class TempTable {
    public:
        TempTable(std::vector<TempColumn> columns);

        size_t count_columns() const;
        TempColumn& get_column(size_t num);
        const TempColumn& get_column(size_t num) const;

        size_t count_rows() const;
        const TempRow* get_row_by_num(size_t num) const;
        TempRow* get_row_by_num(size_t num);

        std::vector<ValueType> get_types() const;

        void insert_row(TempRow row);

    private:
        std::vector<TempColumn> columns;
        std::vector<std::unique_ptr<TempRow>> rows;
};

std::ostream& operator<<(std::ostream& out, const TempTable& table);
