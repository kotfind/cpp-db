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
