#pragma once

#include "concepts.hpp"
#include "result.hpp"

#include <string_view>

namespace parser {
    template<typename F>
    requires requires(F func) {
        { func() } -> is_parser;
    }
    class ParseLazy {
        public:
            using parser_t = std::invoke_result_t<F>;
            using type = typename parser_t::type;
            using result = ParseResult<type>;

            ParseLazy(F func)
              : func(std::move(func))
            {}

            result parse(std::string_view s) const {
                auto parser = std::move(func());
                return parser.parse(s);
            }

        private:
            F func;
    };

    template<typename F>
    requires requires(F func) {
        { func() } -> is_parser;
    }
    ParseLazy<F> lazy(F func) {
        return ParseLazy<F>(std::move(func));
    }
}
