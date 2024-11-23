#include "string.hpp"

#include <cctype>

namespace parser {
    ParseString::ParseString(std::string_view pat, bool ignore_case)
      : pat(pat),
        ignore_case(ignore_case)
    {}

    ParseString::result ParseString::parse(std::string_view s) const {
        if (pat.size() > s.size()) {
            return result::fail({"\"" + std::string(pat) + "\""}, s);
        }

        for (size_t i = 0; i < pat.size(); ++i) {
            auto c1 = ignore_case ? tolower(s[i]) : s[i];
            auto c2 = ignore_case ? tolower(pat[i]) : pat[i];

            if (c1 != c2) {
                return result::fail({std::string(pat)}, s);
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
}
