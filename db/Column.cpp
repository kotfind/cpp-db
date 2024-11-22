#include "Column.hpp"

#include <cassert>
#include <stdexcept>

Column::Column(Ident name_, ValueType type_, std::optional<Value> default_value_)
  : name(std::move(name_)),
    type(type_),
    default_value(std::move(default_value_))
{
    if (default_value.has_value() && default_value->get_type() != type) {
        throw std::runtime_error("failed to create column: column type and default value type are different");
    }
}

const Ident& Column::get_name() const {
    return name;
}

ValueType Column::get_type() const {
    return type;
}

const std::optional<Value>& Column::get_default_value() const {
    return default_value;
}

bool operator==(const Column &lhs, const Column &rhs) {
    return lhs.get_name() == rhs.get_name() && lhs.get_type() == rhs.get_type();
}
