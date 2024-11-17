#pragma once

#include <string>

class Ident {
    public:
        explicit Ident(std::string ident);

        const std::string& get_inner() const;

    private:
        std::string ident;
};

bool operator==(const Ident& lhs, const Ident& rhs);

template <>
struct std::hash<Ident>
{
    std::size_t operator()(const Ident& ident) const;
};
