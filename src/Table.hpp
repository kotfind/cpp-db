#pragma once

#include "Column.hpp"
#include "Ident.hpp"
#include "Row.hpp"
#include "RowInitializerNamed.hpp"
#include "Value.hpp"

#include <memory>
#include <vector>

class Expr;

class Table {
    public:
        Table(Ident name, std::vector<Column> columns);

        const Ident& get_name() const;

        size_t count_columns() const;
        Column& get_column(size_t num);
        Column& get_column(const Ident& ident);
        const Column& get_column(size_t num) const;
        const Column& get_column(const Ident& ident) const;

        size_t count_rows() const;
        const Row* get_row_by_id(size_t id) const;
        Row* get_row_by_id(size_t id);

        size_t get_coulmn_num_by_name(const Ident& name) const;

        std::vector<Row*> select_rows(const Expr& cond) const;

        std::vector<Row*> update_rows(const std::vector<std::pair<Ident, Expr>>& assignments, const Expr& cond);

        std::vector<ValueType> get_types() const;

        void push_row_named(RowInitializerNamed initializer);
        void push_row_positioned(RowInitializerPositioned initializer);

    private:
        Ident name;
        std::vector<Column> columns;
        std::unordered_map<size_t, std::unique_ptr<Row>> rows;
        size_t last_row_id;
};
