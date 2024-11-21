#pragma once

#include "result.hpp"

#include <string_view>
#include <tuple>

namespace parser {
    class ParseDummy {
        public:
            using type = std::tuple<>;
            using result = ParseResult<type>;

            result parse(std::string_view s) const;
    };

    ParseDummy dummy();
}
