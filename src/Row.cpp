#include "Row.hpp"

#include <cassert>
#include <format>
#include <sstream>
#include <stdexcept>

#include "Table.hpp"
#include "Value.hpp"

Row::Row(Table* table_, size_t id_, std::vector<Value> data_)
  : table(table_),
    id(id_),
    data(std::move(data_))
{}

Row::Row(Table* table, size_t id, RowInitializerNamed initializer) {
    // TODO
}

Row::Row(Table* table_, size_t id_, RowInitializerPositioned initializer_)
  : table(table_),
    id(id_),
    data()
{
    if (table->get_columns().size() < initializer_.get_values().size()) {
        throw std::invalid_argument("failed to create row: too many fields in initializer");
    }

    // Add columns to initializer_ if there are too few
    size_t size = table->get_columns().size();
    for (size_t i = initializer_.get_values().size(); i < size; ++i) {
        initializer_.push_value(std::optional<Value>());
    }

    for (size_t i = 0; i < size; ++i) {
        auto value = initializer_.get_values()[i];
        auto col = table->get_columns()[i];

        if (value.has_value()) {
            if (col.get_type() != value->get_type()) {
                std::stringstream ss;
                ss  << "failed to create row: field " << col.get_name()
                    << " has unexpected type " << value->get_type()
                    << ", expeted " << col.get_type();
                throw std::invalid_argument(ss.str());
            }

            data.push_back(*value);
        } else {
            if (!col.get_default_value().has_value()) {
                std::stringstream ss;
                ss  << "failed to create row: neither value nor default value was specified for field " << col.get_name();
                throw std::invalid_argument(ss.str());
            }

            data.push_back(*col.get_default_value());
        }
    }
}

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

Table* Row::get_table() const {
    return table;
}
