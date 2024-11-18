#include "Row.hpp"

#include <cassert>
#include "Value.hpp"

Row::Row(size_t id_, std::vector<Value> data_)
  : id(id_),
    data(std::move(data_))
    
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

size_t Row::get_id() const {
    return id;
}
