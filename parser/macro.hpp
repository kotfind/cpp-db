#pragma once

// Note: inline allows creating parsers in headers
#define PARSER(name, ...) \
    inline auto name(__VA_ARGS__) { \
        using namespace parser; \
        return \
        /* PARSER BODY HERE */

#define END_PARSER \
        /* PARSER BODY HERE */ \
        ; \
    };
