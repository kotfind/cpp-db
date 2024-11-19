#pragma once

#include "result.hpp"

#include <string_view>

namespace parser {
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
}
