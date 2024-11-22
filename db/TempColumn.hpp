#pragma once

#include <string>

enum class ValueType;

class TempColumn {
    public:
        TempColumn(std::string name, ValueType type);

        const std::string& get_name() const;

        ValueType get_type() const;

    private:
        std::string name;
        ValueType type;
};

bool operator==(const TempColumn& lhs, const TempColumn& rhs);
