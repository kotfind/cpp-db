#include "Row.hpp"

#include <cassert>
#include "Value.hpp"

Row::Row(std::vector<Value> data)
  : data(std::move(data))
{}

size_t Row::size() const {
    return data.size();
}

const Value& Row::operator[](size_t idx) const {
    assert(idx < size());
    return data[idx];
}

Value& Row::operator[](size_t idx) {
    assert(idx < size());
    return data[idx];
}

std::vector<ValueType> Row::get_types() const {
    std::vector<ValueType> ans;
    ans.reserve(data.size());

    for (auto value : data) {
        ans.push_back(value.get_type());
    }
    
    return ans;
}
