#pragma once

#include "Value.hpp"

#include <cstddef>
#include <vector>

class Value;
class Table;
class Ident;
class Row;

class TempRow {
    public:
        TempRow(std::vector<Row*> parent_rows, std::vector<Value> data);

        size_t size() const;

        const Value& operator[](size_t idx) const;

        std::vector<ValueType> get_types() const;

        const std::vector<Value>& get_data() const;

        const std::vector<Row*>& get_parent_rows() const;

    private:
        std::vector<Value> data;
        std::vector<Row*> parent_rows;
};
