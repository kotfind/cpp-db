#include "Column.hpp"

Column::Column(Ident name, ValueType type)
  : name(std::move(name)),
    type(type)
{}

const Ident& Column::get_name() const {
    return name;
}

ValueType Column::get_type() const {
    return type;
}

bool operator==(const Column &lhs, const Column &rhs) {
    return lhs.get_name() == rhs.get_name() && lhs.get_type() == rhs.get_type();
}
