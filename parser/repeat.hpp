#pragma once

#include "result.hpp"

#include <limits>
#include <string_view>
#include <vector>

namespace parser {
    template<typename P>
    class ParseRepeat {
        public:
            using type = std::vector<typename P::type>;
            using result = ParseResult<type>;

            ParseRepeat(P parser, size_t min_rep, size_t max_rep)
              : parser(std::move(parser)),
                min_rep(min_rep),
                max_rep(max_rep)
            {}

            result parse(std::string_view s) const {
                type ans;
                for (size_t i = 0; i < max_rep; ++i) {
                    auto res = parser.parse(s);
                    if (res.is_fail()) {
                        if (i < min_rep) {
                            return result::fail();
                        } else {
                            return result::ok(ans, s);
                        }
                    }
                    ans.push_back(std::move(res.value()));
                    s = res.str();
                }
                return result::ok(ans, s);
            }

        private:
            P parser;
            size_t min_rep;
            size_t max_rep;
    };

    /// Parses sequence that repeats from min_rep to max_rep times (inclusive)
    template<typename P>
    ParseRepeat<P> rep(P parser, size_t min_rep, size_t max_rep) {
        return ParseRepeat<P>(std::move(parser), min_rep, max_rep);
    }

    /// Parses sequence that repeats exactly reps times
    template<typename P>
    ParseRepeat<P> rep(P parser, size_t reps) {
        return rep(std::move(parser), reps, reps);
    }

    /// Parses sequence that repeats zero or more times
    template<typename P>
    ParseRepeat<P> rep(P parser) {
        return rep_min(std::move(parser), 0);
    }

    /// Parses sequence that repeats one or more times
    template<typename P>
    ParseRepeat<P> rep1(P parser) {
        return rep_min(std::move(parser), 1);
    }

    /// Parses sequence that repeats from min_rep or more times
    template<typename P>
    ParseRepeat<P> rep_min(P parser, size_t min_rep) {
        return rep(std::move(parser), min_rep, std::numeric_limits<size_t>::max());
    }

    /// Parses sequence that repeats from max_rep or less times
    template<typename P>
    ParseRepeat<P> rep_max(P parser, size_t max_rep) {
        return rep(std::move(parser), 0, max_rep);
    }
}
