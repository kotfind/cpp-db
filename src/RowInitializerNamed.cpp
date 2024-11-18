#include "RowInitializerNamed.hpp"
#include <utility>

RowInitializerNamed::RowInitializerNamed(std::vector<std::pair<Ident, Value>> values_)
   : values(std::move(values_))
{}

RowInitializerNamed::RowInitializerNamed()
   : values()
{}

void RowInitializerNamed::push_value(Ident name, Value value) {
    values.push_back({name, value});
}

const std::vector<std::pair<Ident, Value>>& RowInitializerNamed::get_values() const {
    return values;
}
