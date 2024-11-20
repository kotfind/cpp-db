#pragma once

#include "result.hpp"
#include <concepts>
#include <string_view>
#include <type_traits>

namespace parser {
    template<typename P>
    concept is_parser =
        requires {
            typename P::type;
            typename P::result;
        } &&
        requires(P parser, std::string_view s) {
            { parser.parse(s) } -> std::same_as<typename P::result>;
        } &&
        std::is_same<typename P::result, ParseResult<typename P::type>>::value;

    template<typename P, typename T>
    concept is_parser_of = is_parser<P> && std::is_same<typename P::type, T>::value;
}
