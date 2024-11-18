#pragma once

#include "RowInitializerNamed.hpp"
#include "RowInitializerPositioned.hpp"
#include "Value.hpp"

#include <cstddef>
#include <vector>

class Value;
class Table;

class Row {
    public:
        size_t get_id() const;

        Table* get_table() const;

        size_t size() const;

        const Value& operator[](size_t idx) const;
        Value& operator[](size_t idx);

        std::vector<ValueType> get_types() const;

        const std::vector<Value>& get_data() const;
        std::vector<Value>& get_data();

    private:
        /// Row is created from table
        Row(Table* table, size_t row_id, std::vector<Value> data);

        Row(Table* table, size_t row_id, RowInitializerNamed initializer);

        Row(Table* table, size_t row_id, RowInitializerPositioned initializer);

        Table* table;
        size_t id;
        std::vector<Value> data;

    friend Table;
};
