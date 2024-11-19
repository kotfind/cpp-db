#include "atoms.hpp"

#include <cctype>
#include <limits>

ParseChar::ParseChar(char min, char max)
  : min(min),
    max(max)
{}

ParseChar::result ParseChar::parse(std::string_view s) {
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


ParseString::ParseString(std::string_view pat, bool ignore_case)
  : pat(pat),
    ignore_case(ignore_case)
{}

ParseString::result ParseString::parse(std::string_view s) {
    if (pat.size() < s.size()) {
        return result::fail();
    }

    for (size_t i = 0; i < pat.size(); ++i) {
        auto c1 = ignore_case ? tolower(s[i]) : s[i];
        auto c2 = ignore_case ? tolower(pat[i]) : pat[i];

        if (c1 != c2) {
            return result::fail();
        }
    }

    return result::ok(
        s.substr(0, pat.size()),
        s.substr(pat.size())
    );
}

ParseString s(std::string_view pat) {
    return ParseString(pat, false);
}

ParseString S(std::string_view pat) {
    return ParseString(pat, true);
}
