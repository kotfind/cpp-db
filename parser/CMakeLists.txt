set(TARGET parser)

add_library(${TARGET} STATIC)

target_sources(${TARGET}
    PRIVATE
        main.cpp
)

target_include_directories(${TARGET}
    PRIVATE
        .
    PUBLIC
        include
)

set_target_properties(${TARGET} PROPERTIES PUBLIC_HEADER
    include/test_utils.hpp
)

add_subdirectory(tests)
