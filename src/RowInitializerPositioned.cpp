#include "RowInitializerPositioned.hpp"

RowInitializerPositioned::RowInitializerPositioned(std::vector<std::optional<Value>> values_)
  : values(std::move(values_))
{}

RowInitializerPositioned::RowInitializerPositioned()
  : values()
{}

void RowInitializerPositioned::push_value(std::optional<Value> value) {
    values.push_back(value);
}

const std::vector<std::optional<Value>>& RowInitializerPositioned::get_values() const {
    return values;
}
