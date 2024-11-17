#pragma once

#include "Value.hpp"
#include <cstddef>
#include <initializer_list>
#include <vector>

class Value;

class Row {
    public:
        Row(std::vector<Value> data);

        size_t size() const;

        const Value& operator[](size_t idx) const;

        Value& operator[](size_t idx);

        std::vector<ValueType> get_types() const;

    private:
        std::vector<Value> data;
};

