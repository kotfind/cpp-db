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

const std::vector<Value>& Row::get_data() const {
    return data;
}

std::vector<Value>& Row::get_data() {
    return data;
}

bool operator==(const Row &lhs, const Row &rhs) {
    assert(lhs.get_types() == rhs.get_types());
    return lhs.get_data() == rhs.get_data();
}
