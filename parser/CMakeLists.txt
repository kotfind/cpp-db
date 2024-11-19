set(TARGET parser)

add_library(${TARGET} STATIC)

target_sources(${TARGET}
    PRIVATE
        string.cpp
        char.cpp
)

target_include_directories(${TARGET}
    PRIVATE
        .
    PUBLIC
        include
)

set_target_properties(${TARGET} PROPERTIES PUBLIC_HEADER
    include/parser.hpp
)

target_link_libraries(${TARGET} test_utils)

add_subdirectory(tests)
