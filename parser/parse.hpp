#pragma once

#include "concepts.hpp"

#include <optional>
#include <sstream>
#include <stdexcept>
#include <string_view>

namespace parser {
    /// Tries to parse WHOLE string_view with parser. Returns std::nullopt on fail
    template<is_parser P>
    std::optional<typename P::type> parse_opt(const P& p, std::string_view s) {
        auto res = p.parse(s);
        if (res.is_ok() && res.str().empty()) {
            return std::optional<typename P::type>(std::move(res.value()));
        }
        return std::nullopt;
    }

    namespace __impl {
        std::string form_unparsed_tail(std::string_view tail);
    }

    /// Tries to parse WHOLE string_view with parser. Throws std::runtime_error on fail
    template<is_parser P>
    typename P::type parse(const P& p, std::string_view s) {
        auto res = p.parse(s);

        if (res.is_fail()) {
            std::stringstream ss;
            ss << "parse failed:";
            ss << "\nunparsed tail: " << __impl::form_unparsed_tail(res.str()) << '\n';
            ss << "\nexpected: ";

                // " expected ";
            for (size_t i = 0; i < res.expected_set().size(); ++i) {
                if (i != 0) {
                    ss << " or ";
                }
                ss << res.expected_set()[i];
            }

            throw std::runtime_error(ss.str());
        }

        if (!res.str().empty()) {
            std::stringstream ss;
            ss << "parse failed: part of string was not parsed";
            ss << "\nunparsed tail: " << __impl::form_unparsed_tail(res.str()) << '\n';

            throw std::runtime_error(ss.str());
        }

        return res.value();
    }
}
