#pragma once

#include "Ident.hpp"
#include "Value.hpp"

#include <utility>
#include <vector>

class RowInitializerNamed {
    public:
        RowInitializerNamed(std::vector<std::pair<Ident, Value>> values);
        RowInitializerNamed();

        void push_value(Ident name, Value value);

        const std::vector<std::pair<Ident, Value>>& get_values() const;

    private:
        std::vector<std::pair<Ident, Value>> values;
};
