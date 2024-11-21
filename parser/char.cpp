#include "char.hpp"
#include "result.hpp"

#include <limits>
#include <string>

namespace parser {
    ParseChar::ParseChar(char min, char max)
      : min(min),
        max(max)
    {}

    ParseChar::result ParseChar::parse(std::string_view s) const {
        if (s.empty()) {
            return result::fail({get_expected()}, s);
        }

        char c = s[0];
        s.remove_prefix(1);

        if (min <= c && c <= max) {
            return result::ok(c, s);
        } else {
            return result::fail({get_expected()}, s);
        }
    }

    std::string ParseChar::get_expected() const {
        std::string expected;

        bool has_min = min != std::numeric_limits<char>::min();
        bool has_max = max != std::numeric_limits<char>::max();

        std::string min_str(1, min);
        std::string max_str(1, max);

        if (min == max) {
            expected = "'" + std::string(1, min) + "'";
        } else if (!has_min && !has_min) {
            expected = "CHAR";
        } else if (has_min && !has_min) {
            expected = min_str + "..";
        } else if (!has_min && has_min) {
            expected = ".." + max_str;
        } else {
            expected = min_str + ".." + max_str;
        }

        return expected;
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
