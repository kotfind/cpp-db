#include "RowInitializerNamed.hpp"

#include <cassert>
#include <sstream>
#include <stdexcept>
#include <utility>

RowInitializerNamed::RowInitializerNamed(std::unordered_map<Ident, Value> values_)
   : values(std::move(values_))
{}

RowInitializerNamed::RowInitializerNamed()
   : values()
{}

void RowInitializerNamed::push_value(Ident name, Value value) {
    if (!values.contains(name)) {
        std::stringstream ss;
        ss << "failed to push_value to RowInitializerNamed: redefinition of " << name;
        throw std::invalid_argument(ss.str());
    }
    values.insert({name, value});
}

const std::unordered_map<Ident, Value>& RowInitializerNamed::get_values() const {
    return values;
}
