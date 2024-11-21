#pragma once

#include "dummy.hpp"
#include "result.hpp"
#include "concepts.hpp"

#include <limits>
#include <string_view>
#include <vector>

namespace parser {
    template<is_parser P, is_parser S = ParseDummy>
    class ParseRepeat {
        public:
            using type = std::vector<typename P::type>;
            using result = ParseResult<type>;

            ParseRepeat(P parser_, S separator_parser_, size_t min_rep, size_t max_rep)
              : parser(std::move(parser_)),
                separator_parser(std::move(separator_parser_)),
                min_rep(min_rep),
                max_rep(max_rep)
            {}

            result parse(std::string_view s) const {
                type ans;

                while (ans.size() < max_rep) {
                    auto s_ = s;

                    if (!ans.empty()) {
                        auto sep_res = separator_parser.parse(s);
                        if (sep_res.is_fail()) break;
                        s_ = sep_res.str();
                    }

                    auto res = parser.parse(s_);
                    if (res.is_fail()) break;

                    ans.push_back(std::move(res.value()));
                    s = res.str();
                }

                if (ans.size() < min_rep) {
                    return result::fail();
                } else {
                    return result::ok(ans, s);
                }
            }

        private:
            P parser;
            S separator_parser;
            size_t min_rep;
            size_t max_rep;
    };

    /// Parses sequence that repeats from min_rep to max_rep times (inclusive)
    template<is_parser P, is_parser S>
    ParseRepeat<P, S> betw(P parser, S separator_parser, size_t min_rep, size_t max_rep) {
        return ParseRepeat<P, S>(std::move(parser), std::move(separator_parser), min_rep, max_rep);
    }

    /// Parses sequence that repeats exactly reps times
    template<is_parser P, is_parser S>
    ParseRepeat<P, S> betw(P parser, S separator_parser, size_t reps) {
        return betw(std::move(parser), std::move(separator_parser), reps, reps);
    }

    /// Parses sequence that repeats zero or more times
    template<is_parser P, is_parser S>
    ParseRepeat<P, S> betw(P parser, S separator_parser) {
        return betw_min(std::move(parser), std::move(separator_parser), 0);
    }

    /// Parses sequence that repeats one or more times
    template<is_parser P, is_parser S>
    ParseRepeat<P, S> betw1(P parser, S separator_parser) {
        return betw_min(std::move(parser), std::move(separator_parser), 1);
    }

    /// Parses sequence that repeats from min_rep or more times
    template<is_parser P, is_parser S>
    ParseRepeat<P, S> betw_min(P parser, S separator_parser, size_t min_rep) {
        return betw(std::move(parser), std::move(separator_parser), min_rep, std::numeric_limits<size_t>::max());
    }

    /// Parses sequence that repeats from max_rep or less times
    template<is_parser P, is_parser S>
    ParseRepeat<P, S> betw_max(P parser, S separator_parser, size_t max_rep) {
        return betw(std::move(parser), std::move(separator_parser), 0, max_rep);
    }

    /// Parses sequence that repeats from min_rep to max_rep times (inclusive)
    template<is_parser P>
    ParseRepeat<P> rep(P parser, size_t min_rep, size_t max_rep) {
        return betw(std::move(parser), dummy(), min_rep, max_rep);
    }

    /// Parses sequence that repeats exactly reps times
    template<is_parser P>
    ParseRepeat<P> rep(P parser, size_t reps) {
        return betw(std::move(parser), dummy(), reps);
    }

    /// Parses sequence that repeats zero or more times
    template<is_parser P>
    ParseRepeat<P> rep(P parser) {
        return betw(std::move(parser), dummy());
    }

    /// Parses sequence that repeats one or more times
    template<is_parser P>
    ParseRepeat<P> rep1(P parser) {
        return betw1(std::move(parser), dummy());
    }

    /// Parses sequence that repeats from min_rep or more times
    template<is_parser P>
    ParseRepeat<P> rep_min(P parser, size_t min_rep) {
        return betw_min(std::move(parser), dummy(), min_rep);
    }

    /// Parses sequence that repeats from max_rep or less times
    template<is_parser P>
    ParseRepeat<P> rep_max(P parser, size_t max_rep) {
        return betw_max(std::move(parser), dummy(), max_rep);
    }
}
