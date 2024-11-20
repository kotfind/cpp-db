#pragma once

#include "result.hpp"
 
#include <memory>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <type_traits>

namespace parser {

    template<typename T>
    class Parser {
        public:
            using type = T;
            using result = ParseResult<type>;

        private:
            class AbstractParser {
                public:
                    virtual ~AbstractParser() {}

                    virtual result parse(std::string_view s) const = 0;
            };

            template<typename P>
            class Inner : public AbstractParser {
                static_assert(std::is_same<typename P::type, T>::value);

                public:
                    Inner(P parser_)
                      : parser(std::move(parser_))
                    {}

                    result parse(std::string_view s) const {
                        return parser.parse(s);
                    }

                private:
                    P parser;
            };

        public:
            template<typename P>
            Parser(P parser_)
              : parser(std::make_shared<Inner<P>>(std::move(parser_)))
            {}

            result parse(std::string_view s) const {
                return parser->parse(s);
            }

            std::shared_ptr<AbstractParser> parser;
    };

    /// Tries to parse WHOLE string_view with parser. Returns std::nullopt on fail
    template<typename P>
    std::optional<typename P::type> parse_opt(const P& p, std::string_view s) {
        auto res = p.parse(s);
        if (res.is_ok() && res.str().empty()) {
            return std::optional<typename P::type>(std::move(res.value()));
        }
        return std::nullopt;
    }


    /// Tries to parse WHOLE string_view with parser. Throws std::runtime_error on fail
    template<typename P>
    typename P::type parse(const P& p, std::string_view s) {
        auto res = parse_opt(p, s);
        if (res.has_value()) {
            return std::move(*res);
        }
        throw std::runtime_error("parse failed");
    }
}
