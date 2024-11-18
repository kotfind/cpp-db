#include "Ident.hpp"

#include <cassert>
#include <cctype>
#include <functional>
#include <string>

Ident::Ident(std::string ident_)
  : ident(std::move(ident_))
{
    assert(!ident.empty());
    assert(ident[0] == '_' || std::isalpha(ident[0]));
    for (char c : ident) {
        assert(c == '_' || std::isalnum(c));
    }
}

const std::string& Ident::get_inner() const {
    return ident;
}

bool operator==(const Ident& lhs, const Ident& rhs) {
    return lhs.get_inner() == rhs.get_inner();
}

std::size_t std::hash<Ident>::operator()(const Ident& ident) const {
    return std::hash<std::string>()(ident.get_inner());
}

std::ostream& operator<<(std::ostream& out, const Ident& ident) {
    out << ident.get_inner();
    return out;
}
