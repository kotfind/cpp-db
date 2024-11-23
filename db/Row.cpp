#include "Row.hpp"

#include <cassert>
#include <optional>
#include <sstream>
#include <stdexcept>

#include "Column.hpp"
#include "Table.hpp"
#include "Value.hpp"

/// Returns value that should be pushed to row if possible.
/// Throws warning otherwise.
Value resolve_row_value(size_t id, const Column& col, std::optional<Value> value) {
    if (value.has_value()) {
        if (col.get_type() != value->get_type()) {
            std::stringstream ss;
            ss  << "failed to create row: field " << col.get_name()
                << " has unexpected type " << value->get_type()
                << ", expeted " << col.get_type();
            throw std::invalid_argument(ss.str());
        }

        return *value;
    } else {
        auto def = col.get_default_value_ext(id);
        if (!def.has_value()) {
            std::stringstream ss;
            ss  << "failed to create row: neither value nor default value was specified for field " << col.get_name();
            throw std::invalid_argument(ss.str());
        }

        return *def;
    }
}

Row::Row(Table* table_, size_t id_, std::vector<Value> data_)
  : table(table_),
    id(id_),
    data(std::move(data_))
{}

Row::Row(Table* table_, size_t id_, RowInitializerNamed initializer_)
  : table(table_),
    id(id_),
    data()
{
    data.reserve(table->count_columns());
    auto init_vals = std::move(initializer_.get_values());
    size_t values_used = 0;

    for (size_t col_id = 0; col_id < table->count_columns(); ++col_id) {
        auto& col = table->get_column(col_id);
        if (init_vals.contains(col.get_name())) {
            data.push_back(resolve_row_value(
                id,
                col,
                std::optional<Value>(std::move(init_vals.at(col.get_name())))
            ));
            ++values_used;
        } else {
            data.push_back(resolve_row_value(
                id,
                col,
                std::optional<Value>()
            ));
        }
    }

    if (values_used < init_vals.size()) {
        throw std::invalid_argument("failed to create row: some fields in initializer are undefined");
    }
}

Row::Row(Table* table_, size_t id_, RowInitializerPositioned initializer_)
  : table(table_),
    id(id_),
    data()
{
    if (table->count_columns() < initializer_.get_values().size()) {
        throw std::invalid_argument("failed to create row: too many fields in initializer");
    }

    size_t size = table->count_columns();
    data.reserve(size);

    // Add columns to initializer_ if there are too few
    for (size_t i = initializer_.get_values().size(); i < size; ++i) {
        initializer_.push_value(std::optional<Value>());
    }

    for (size_t i = 0; i < size; ++i) {
        auto value = initializer_.get_values()[i];
        auto col = table->get_column(i);

        if (value.has_value()) {
            if (col.get_type() != value->get_type()) {
                std::stringstream ss;
                ss  << "failed to create row: field " << col.get_name()
                    << " has unexpected type " << value->get_type()
                    << ", expeted " << col.get_type();
                throw std::invalid_argument(ss.str());
            }

            data.push_back(std::move(*value));
        } else {
            auto def = col.get_default_value_ext(id);
            if (!def.has_value()) {
                std::stringstream ss;
                ss  << "failed to create row: neither value nor default value was specified for field " << col.get_name();
                throw std::invalid_argument(ss.str());
            }

            data.push_back(*def);
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

const Value& Row::operator[](const Ident& ident) const {
    return (*this)[get_table()->get_coulmn_num_by_name(ident)];
}

Value& Row::operator[](const Ident& ident) {
    return (*this)[get_table()->get_coulmn_num_by_name(ident)];
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

VarMap Row::to_vars() const {
    VarMap vars;
    for (size_t col_id = 0; col_id < get_table()->count_columns(); ++col_id) {
        vars.insert({get_table()->get_column(col_id).get_name(), (*this)[col_id]});
    }
    return vars;
}
