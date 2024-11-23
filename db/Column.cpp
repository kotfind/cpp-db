#include "Column.hpp"

#include <cassert>
#include <optional>
#include <stdexcept>

Column::Column(
    Ident name_,
    ValueType type_,
    std::optional<Value> default_value_,
    ColumnAttrs attrs_
) : name(std::move(name_)),
    type(type_),
    default_value(std::move(default_value_)),
    attrs(attrs_)
{
    if (default_value.has_value()) {
        if (default_value->get_type() != type) {
            throw std::runtime_error("failed to create column: column type and default value type are different");
        }
        if (attrs & COL_ATTR_AUTOINCREMENT) {
            throw std::runtime_error("failed to create column: both autoincrement and default value are not allowed");

            if (type != ValueType::INT) {
                throw std::runtime_error("failed to create column: autoincrement table must be of type int");
            }
        }
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

std::optional<Value> Column::get_default_value_ext(size_t id) const {
    if (get_default_value().has_value()) {
        return get_default_value();
    }
    if (attrs & COL_ATTR_AUTOINCREMENT) {
        return {Value::from_int(id)};
    }
    return std::nullopt;
}

bool operator==(const Column &lhs, const Column &rhs) {
    return lhs.get_name() == rhs.get_name() && lhs.get_type() == rhs.get_type();
}
