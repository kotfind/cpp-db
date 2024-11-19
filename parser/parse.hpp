#include <optional>
#include <stdexcept>
#include <string_view>

namespace parser {
    /// Tries to parse WHOLE string_view with parser. Returns std::nullopt on fail
    template<typename P>
    std::optional<typename P::type> parse_opt(P p, std::string_view s) {
        auto res = p.parse(s);
        if (res.is_ok() && res.str().empty()) {
            return std::optional<typename P::type>(std::move(res.value()));
        }
        return std::nullopt;
    }


    /// Tries to parse WHOLE string_view with parser. Throws std::runtime_error on fail
    template<typename P>
    typename P::type parse(P p, std::string_view s) {
        auto res = parse_opt(std::move(p), s);
        if (res.has_value()) {
            return std::move(*res);
        }
        throw std::runtime_error("parse failed");
    }
}
