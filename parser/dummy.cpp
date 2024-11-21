#include "dummy.hpp"

namespace parser {
    ParseDummy::result ParseDummy::parse(std::string_view s) const {
        return result::ok({}, s);
    }

    ParseDummy dummy() {
        return ParseDummy {};
    }
}
