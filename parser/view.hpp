#pragma once

#include "result.hpp"

#include <cassert>
#include <string_view>

namespace parser {
    template<typename P>
    class ParseView {
        public:
            using type = std::string_view;
            using result = ParseResult<type>;

            ParseView(P parser)
              : parser(std::move(parser))
            {}

            result parse(std::string_view s) {
                auto res = parser.parse(s);
                if (res.is_fail()) {
                    return result::fail();
                }
                assert(s.ends_with(res.str()));
                s.remove_suffix(res.str().size());
                return result::ok(s, res.str());
            }

        private:
            P parser;
    };

    template<typename P>
    ParseView<P> view(P parser) {
        return ParseView<P>(std::move(parser));
    }
}
