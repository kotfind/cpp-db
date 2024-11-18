#pragma once

#include "Value.hpp"

#include <cstddef>
#include <vector>

class Value;

class Row {
    public:
        Row(size_t row_id, std::vector<Value> data);

        size_t size() const;

        const Value& operator[](size_t idx) const;
        Value& operator[](size_t idx);

        std::vector<ValueType> get_types() const;

        const std::vector<Value>& get_data() const;
        std::vector<Value>& get_data();

        size_t get_id() const;

    private:
        size_t id;
        std::vector<Value> data;
};
