#include "TempColumn.hpp"

#include "Value.hpp"

#include <cassert>
#include <string>

TempColumn::TempColumn(std::string name_, ValueType type_)
  : name(std::move(name_)),
    type(type_)
{}

const std::string& TempColumn::get_name() const {
    return name;
}

ValueType TempColumn::get_type() const {
    return type;
}

bool operator==(const TempColumn &lhs, const TempColumn &rhs) {
    return lhs.get_name() == rhs.get_name() && lhs.get_type() == rhs.get_type();
}
