#pragma once

#include "Ident.hpp"
#include "Value.hpp"

#include <unordered_map>

class RowInitializerNamed {
    public:
        RowInitializerNamed(std::unordered_map<Ident, Value> values);
        RowInitializerNamed();

        void push_value(Ident name, Value value);

        const std::unordered_map<Ident, Value>& get_values() const;

    private:
        std::unordered_map<Ident, Value> values;
};
