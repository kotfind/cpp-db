#pragma once

#include "result.hpp"

#include "concepts.hpp"
 
#include <memory>
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
              : parser(
                    std::make_shared<std::unique_ptr<AbstractParser>>(
                        std::unique_ptr<AbstractParser>(
                            std::make_unique<Inner<P>>(std::move(parser_))
                        )
                    )
                )
            {}

            /// Creates "dummy" parser. Useful for creating recursive parsers
            Parser()
              : parser(
                    std::make_shared<std::unique_ptr<AbstractParser>>(
                        std::unique_ptr<AbstractParser>(nullptr)
                    )
                )
            {}

            result parse(std::string_view s) const {
                if (is_initialized()) {
                    return (*parser)->parse(s);
                } else {
                    throw std::runtime_error("parser was not initialized");
                }
            }

            template<is_parser_for<T> P>
            void initialize(P parser_) {
                if (is_initialized()) {
                    throw std::runtime_error("parser was already initialized");
                }
                parser->reset(new Inner<P>(std::move(parser_)));
            }

        private:

            bool is_initialized() const {
                return parser->get() != nullptr;
            }

            // Double indirection allows creating recursive parsers
            std::shared_ptr<std::unique_ptr<AbstractParser>> parser;
    };
}
