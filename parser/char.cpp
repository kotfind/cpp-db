#include "char.hpp"

#include <limits>

namespace parser {
    ParseChar::ParseChar(char min, char max)
      : min(min),
        max(max)
    {}

    ParseChar::result ParseChar::parse(std::string_view s) const {
        if (s.empty()) {
            return result::fail();
        }

        char c = s[0];
        s.remove_prefix(1);

        if (min <= c && c <= max) {
            return result::ok(c, s);
        } else {
            return result::fail();
        }
    }

    ParseChar c(char min, char max) {
        return ParseChar(min, max);
    }

    ParseChar c() {
        return ParseChar(std::numeric_limits<char>::min(), std::numeric_limits<char>::max());
    }

    ParseChar c(char ch) {
        return ParseChar(ch, ch);
    }
}
