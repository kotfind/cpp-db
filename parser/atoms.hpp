#pragma once

#include "result.hpp"

#include <string_view>

class ParseChar {
    public:
        using type = char;
        using result = ParseResult<type>;

        ParseChar(char min, char max);

        result parse(std::string_view s);

    private:
        char min;
        char max;
};

/// Parses char in inclusive range [min; max]
ParseChar c(char min, char max);

/// Parses any char
ParseChar c();

/// Parses exact char
ParseChar c(char ch);


class ParseString {
    public:
        using type = std::string_view;
        using result = ParseResult<type>;

        ParseString(std::string_view pat, bool ignore_case = false);

        result parse(std::string_view s);

    private:
        std::string_view pat;
        bool ignore_case;
};

/// Parses exact string (case sensitive)
ParseString s(std::string_view pat);

/// Parses exact string (ignores case)
ParseString S(std::string_view pat);
