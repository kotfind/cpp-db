#pragma once

#include "Value.hpp"
#include "Ident.hpp"

class Column {
    public:
        Column(Ident name, ValueType type);

        const Ident& get_name() const;

        ValueType get_type() const;

    private:
        Ident name;
        ValueType type;
};

bool operator==(const Column &lhs, const Column &rhs);
