#pragma once

#include "result.hpp"

#include "concepts.hpp"
 
#include <memory>
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

            template<is_parser_for<T> P>
            requires std::is_same<typename P::type, T>::value
            class Inner : public AbstractParser {

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
            template<is_parser_for<T> P>
            Parser(P parser_)
              : parser(std::make_shared<Inner<P>>(std::move(parser_)))
            {}

            result parse(std::string_view s) const {
                return parser->parse(s);
            }

            std::shared_ptr<AbstractParser> parser;
    };
}
