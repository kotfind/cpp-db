#include "TempRow.hpp"
#include <cassert>

TempRow::TempRow(std::vector<Row*> parent_rows_, std::vector<Value> data_)
  : parent_rows(std::move(parent_rows_)),
    data(std::move(data_))
{}

size_t TempRow::size() const {
    return data.size();
}

const Value& TempRow::operator[](size_t idx) const {
    assert(idx < data.size());
    return data[idx];
}

std::vector<ValueType> TempRow::get_types() const {
    std::vector<ValueType> ans;
    ans.reserve(data.size());

    for (auto value : data) {
        ans.push_back(value.get_type());
    }
    
    return ans;
}

const std::vector<Value>& TempRow::get_data() const {
    return data;
}

const std::vector<Row*>& TempRow::get_parent_rows() const {
    return parent_rows;
}
