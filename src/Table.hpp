#pragma once

#include "Column.hpp"
#include "Ident.hpp"
#include "Row.hpp"
#include "RowInitializerNamed.hpp"
#include "Value.hpp"

#include <memory>
#include <vector>

class Table {
    public:
        Table(Ident name, std::vector<Column> columns);

        const Ident& get_name() const;

        const std::vector<Column>& get_columns() const;
        std::vector<Column>& get_columns();

        const std::vector<std::unique_ptr<Row>>& get_rows() const;
        std::vector<std::unique_ptr<Row>>& get_rows();

        std::vector<ValueType> get_types() const;

        void push_row_named(RowInitializerNamed initializer);
        void push_row_positioned(RowInitializerPositioned initializer);

    private:
        Ident name;
        std::vector<Column> columns;
        std::vector<std::unique_ptr<Row>> rows;
        size_t last_row_id;
};
