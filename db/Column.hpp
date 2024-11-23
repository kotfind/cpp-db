#pragma once

#include "Value.hpp"
#include "Ident.hpp"
#include <optional>

enum ColumnAttrs {
    COL_ATTR_NONE = 0,
    COL_ATTR_UNIQE = 1 << 1,
    COL_ATTR_AUTOINCREMENT = 1 << 2,
};

class Column {
    public:
        Column(
            Ident name,
            ValueType type,
            std::optional<Value> default_value = std::optional<Value>(),
            ColumnAttrs attrs = COL_ATTR_NONE
        );

        const Ident& get_name() const;

        ValueType get_type() const;

        std::optional<Value> get_default_value_ext(size_t id) const;

    private:
        const std::optional<Value>& get_default_value() const;

        Ident name;
        ValueType type;
        std::optional<Value> default_value;
        ColumnAttrs attrs;
};

bool operator==(const Column& lhs, const Column& rhs);
