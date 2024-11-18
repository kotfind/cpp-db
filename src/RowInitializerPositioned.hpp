#pragma once

#include "Value.hpp"

#include <optional>
#include <vector>

class RowInitializerPositioned {
    public:
        RowInitializerPositioned(std::vector<std::optional<Value>> values);
        RowInitializerPositioned();

        void push_value(std::optional<Value> value);

        const std::vector<std::optional<Value>>& get_values() const;

    private:
        std::vector<std::optional<Value>> values;
};
