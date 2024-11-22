#pragma once

#include "Value.hpp"
#include "Ident.hpp"
#include <optional>

class Column {
    public:
        Column(Ident name, ValueType type, std::optional<Value> default_value = std::optional<Value>());

        const Ident& get_name() const;

        ValueType get_type() const;

        const std::optional<Value>& get_default_value() const;

    private:
        Ident name;
        ValueType type;
        std::optional<Value> default_value;
};

bool operator==(const Column& lhs, const Column& rhs);
